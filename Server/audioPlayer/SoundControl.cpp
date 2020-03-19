/*
SoundControl: Class that allows the to set the volume control.
Inspired from: https://stackoverflow.com/questions/6787318/set-alsa-master-volume-from-c-code

*/
#ifndef SOUND_CONTROL
#define SOUND_CONTROL

#include <alsa/asoundlib.h>
#include <string>
#include <iostream>

class SoundControl
{
  public:

    ~SoundControl()
    {
        delete sound;
        sound = nullptr;
    }
    
    static SoundControl* getInstance()
    {
        if (sound == nullptr)
        {
            sound = new SoundControl();
        }
        return sound;
    }

    /**
     * Method user to initialize the volume control
     **/
    bool initAlsaVolume()
    {
        if (snd_mixer_open(&handle, 0) < 0)
            return false;
        if (snd_mixer_attach(handle, card.c_str()) < 0)
            return false;
        if (snd_mixer_selem_register(handle, NULL, NULL) < 0)
            return false;
        if (snd_mixer_load(handle) < 0)
            return false;

        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, selem_name.c_str());
        elem = snd_mixer_find_selem(handle, sid);

        if (snd_mixer_selem_get_playback_volume_range(elem, &minVolumeRange, &maxVolumeRange) < 0)
            return false;
        if (snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume) < 0)
            return false;
        isMuted = false;
	
        return true;
    }

    /**
     * lower the volume by a percentage
     **/
    void lowerVolume(int pc)
    {
        long vol = ((double)pc / 100) * maxVolumeRange;
        volume -= vol;
        if (volume < minVolumeRange)
            volume = minVolumeRange;

        if (!isMuted)
        {
            if (volume == 0)
                snd_mixer_selem_set_playback_switch_all(elem, 0);
            else
                snd_mixer_selem_set_playback_volume_all(elem, volume);
        }
        
    }

    /**
     * turn the volume up by a percentage
     **/
    void volumeUp(int pc)
    {
        if (volume == 0) {
            snd_mixer_selem_set_playback_switch_all(elem, 1);
            snd_mixer_selem_set_playback_volume_all(elem, volume);
        }

        long vol = ((double)pc / 100) * maxVolumeRange;
        volume += vol;
        
        if (!isMuted)
        {
            if (volume > maxVolumeRange)
                volume = maxVolumeRange;
	        
            snd_mixer_selem_set_playback_volume_all(elem, volume);
        }
    }

    /**
     * mute the volume
     **/
    void muteVolume()
    {
        snd_mixer_selem_set_playback_switch_all(elem, 0);
        isMuted = true;
    }

    /**
     * unmute the volume
     **/
    void unmuteVolume()
    {
        if (isMuted)
        {
	        snd_mixer_selem_set_playback_switch_all(elem, 1);
            snd_mixer_selem_set_playback_volume_all(elem, volume);
            isMuted = false;
        }
    }

    /*
    * ontain mute state of sound
    */
   bool isSoundMuted() { return isMuted; }

    /**
     * Get the volume. Return pourcentage
     **/
    long getVolume()
    {
        if (isMuted)
	    return ((double) (volume - minVolumeRange) / maxVolumeRange) * 100;
 
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
        
	    return ((double) (volume - minVolumeRange) / maxVolumeRange) * 100;
    }

    /**
    * Exit sound control
    **/
    void closeAlsaVolume()
    {
        snd_mixer_close(handle);
    }

  private:

    SoundControl(){};
    static SoundControl* sound;

    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const std::string card = "default";
    const std::string selem_name = "Headphone";
    snd_mixer_elem_t *elem;

    long minVolumeRange;
    long maxVolumeRange;
    long volume;
    bool isMuted;
};
SoundControl* SoundControl::sound = nullptr;

#endif
