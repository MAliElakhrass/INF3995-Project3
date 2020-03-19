/*
AlsaMediaPlayer : Class for audio device setup, libmad mp3 decoding and alsa playback.
                  This class is used by the SongList class to play songs in queue.

                  It was inspired by and based on code and theory from the following links :
                  https://github.com/bbc/audiowaveform/blob/master/src/Mp3AudioFileReader.cpp
                  https://www.linuxjournal.com/article/6735?page=0,2
                  https://lauri.võsandi.com/2013/12/implementing-mp3-player.en.html
*/
#include <memory>
#include <fstream>
#include <vector>

#include <alsa/asoundlib.h>
#include <mad.h>
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../constants.h"

using json = nlohmann::json;

#define STEREO 2
#define MONO 1
#define BAD_SAMPLE_RATE 44100
#define GOOD_SAMPLE_RATE 40000

class AlsaMediaPlayer
{
public:
    AlsaMediaPlayer() {}
  
    /** Method used by main method in Server.cpp to setup sound device
    * 
    *  @return A boolean indicating Server if the device was successfully setup
    */
    bool setUpDevice()
    {
        //open device for playback to default sound card
        if (snd_pcm_open(&pcm_device, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
            return false;

        return true;
    }

    /** Method used by MediaPlayer thread to play next song in line
    * 
    *  @param filePath: The path of the next mp3 song file
    */
    void play(std::string filePath)
    {
        if (!setPlaybackParameters(filePath))
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to setup playback parameters for mp3 file " + filePath);
            return;
        }
        //Initialization of libmad structures for mp3 decoding.
        mad_stream_init(&stream); //stream -> this is the input stream buffer that contains the information that needs to be decoded.
        mad_synth_init(&synth); //synth -> this is the structure that handles PCM (pulse-code modulation) synthesis. It represents analog in digital form.
        mad_frame_init(&frame); //frame -> decoded information will be stored here.

        // Open file and fill buffer
        mp3File.open(filePath, std::ifstream::ate | std::ifstream::binary);
        if (!mp3File.is_open())
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to open mp3 file " + filePath);
            return;
        }
        unsigned long fileSize = mp3File.tellg();
        std::shared_ptr<char> buffer(new char[fileSize]);
        mp3File.seekg(0, std::ios::beg);
        mp3File.read(buffer.get(), fileSize);

        //associate buffer with the libmad stream struct.
        mad_stream_buffer(&stream, (unsigned char*) buffer.get(), fileSize);

        //start playing audio
        decodeAndPlay();

        // Close files and free structs
        mp3File.close();

        mad_stream_finish(&stream);
        mad_synth_finish(&synth);
        mad_frame_finish(&frame);

        //device is not closed until server shutdown. Reset audio device buffer instead.
        snd_pcm_drain(pcm_device);
        snd_pcm_prepare(pcm_device);
    }

    //method used to close device at server shutdown.
    void closeDevice()
    {   
        if (mp3File.is_open())
            mp3File.close();
        mad_stream_finish(&stream);
        mad_synth_finish(&synth);
        mad_frame_finish(&frame);
        snd_pcm_close(pcm_device);
    }

private:
    //alsa structs -> audio playback
    snd_pcm_t* pcm_device;
    snd_pcm_hw_params_t* pcm_device_params;
    snd_pcm_uframes_t frames;
    //libmad structs -> mp3 decoding and pcm synthesizing
    struct mad_stream stream;
    struct mad_frame frame;
    struct mad_synth synth;
    //file stream
    std::ifstream mp3File;

    bool setPlaybackParameters(std::string filePath)
    {
        size_t index = filePath.find_first_not_of(UPLOAD_DIR);
        std::string jsonHeadersPath = JSON_DIR + filePath.substr(index-1) + ".json";

        std::ifstream jsonHeaderFile(jsonHeadersPath);
        if (!jsonHeaderFile)
            return false;
        
        json mp3Headers;
        jsonHeaderFile >> mp3Headers;
        jsonHeaderFile.close();
        
        unsigned int rate = mp3Headers["sampleRate"];
        int mode = mp3Headers["channels"];
        if (mode != MONO && mode != STEREO)
            return false;
        int dir;

        snd_pcm_hw_params_malloc(&pcm_device_params);

        if (snd_pcm_hw_params_any(pcm_device, pcm_device_params) < 0) //default parameters for those not specified
            return false;
        if (snd_pcm_hw_params_set_access(pcm_device, pcm_device_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
            return false;
        if (mode == MONO)
        {
            if (snd_pcm_hw_params_set_channels(pcm_device, pcm_device_params, 1) < 0)
                return false;
        }
        if (mode == STEREO)
        {
            if (snd_pcm_hw_params_set_channels(pcm_device, pcm_device_params, 2) < 0)
                return false;
        }
        if (rate == BAD_SAMPLE_RATE)
            rate = GOOD_SAMPLE_RATE;
        if (snd_pcm_hw_params_set_rate_near(pcm_device, pcm_device_params, &rate, &dir) < 0)
            return false;
        if (snd_pcm_hw_params_set_format(pcm_device, pcm_device_params, SND_PCM_FORMAT_S16_LE) < 0)
            return false;

        
        //send parameters to driver
        if (snd_pcm_hw_params(pcm_device, pcm_device_params) < 0)
            return false;

        snd_pcm_hw_params_free(pcm_device_params);

        return true;
    }
 
    //libmad decoding method
    void decodeAndPlay()
    {
        snd_pcm_sframes_t delay;
        //Decode frame and synthesize loop
        do {
            // Decode frame from the stream
            if (mad_frame_decode(&frame, &stream)) {             
                if (MAD_RECOVERABLE(stream.error)) {
                    continue;
                } else if (stream.error == MAD_ERROR_BUFLEN) {
                    continue;
                } else {
                    break;
                }
            }
            // Synthesize PCM data of frame
            mad_synth_frame(&synth, &frame);

            //send synthesized frame to sound device using alsa
            if (!output(&synth.pcm))
                break;

        } while(snd_pcm_avail(pcm_device) > 0);
    }

    //alsa playback method
    bool output(struct mad_pcm *pcm)
    { 
        int nframes = pcm->length;
        std::vector<char> buffer;
        short sample;
        if (pcm->channels == 2) {
            
            for (int i = 0; i < nframes; i++) {
                //left channel
                sample =  MadFixedToShort(pcm->samples[0][i]);
                buffer.push_back(sample);
                buffer.push_back(sample >> 8);
                //right channel
                sample =  MadFixedToShort(pcm->samples[1][i]);
                buffer.push_back(sample);
                buffer.push_back(sample >> 8);
            }

        } else 
        {
            for (int i = 0; i < nframes; i++) {
                //single channel
                sample =  MadFixedToShort(pcm->samples[0][i]);
                buffer.push_back(sample);
                buffer.push_back(sample >> 8);
            }
        }

        //Use alsa to read from buffer and write in device. Size is in frames.
            int status = snd_pcm_writei(pcm_device, buffer.data(), nframes);
            if (status == -EPIPE) {
                // EPIPE means underrun. Reset audio device buffer.
                snd_pcm_prepare(pcm_device);
            } else if (status < 0) {
                NVJ_LOG->append(NVJ_ERROR, "Error from alsa method writei while playing song.");
                return false;
            }
        return true;
    }

    //This method is used to convert pcm samples from mad_fixed_t struct to short primitive type.
    short MadFixedToShort(mad_fixed_t fixed)
    {
        // Clipping
        if (fixed >= MAD_F_ONE)
        {
            return SHRT_MAX;
        }
        if (fixed <= -MAD_F_ONE)
        {
            return -SHRT_MAX;
        }

        // Conversion
        fixed >>= (MAD_F_FRACBITS - 15);

        return static_cast<short>(fixed);
    }
};