/*
SongList : Data structure class that encapsulates song queue.
           This list stores and accesses songs for playback.
           This list is contained by the Singleton ListContainer class.
*/
#ifndef SONG_LIST
#define SONG_LIST

#include <memory>
#include <list>
#include <semaphore.h>
#include <signal.h>
#include <string>

#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "AlsaMediaPlayer.cpp"

class SongList
{
public:
    SongList() 
    { 
        numberOfSongsUploaded = 0;
        numberOfSongsDeleted = 0;
        totalUploadedLengthInSeconds = 0;
        currentSong = NONE;
        initializeMutex(); 
    }

    /** Method used by userServices/SongListPage.cpp to obtain the song currently playing
    * 
    *  @return the path of the current mp3 song
    */
    std::string getCurrentSong()
    {
        return currentSong;
    }

    /** Method used by MediaPlayer thread to remove current song its done playing
    */
   void noCurrentSong()
   {
       currentSong = NONE;
   }

    /** Method used by MediaPlayer thread to check if there is a song in queue.
    * 
    *  @return A boolean indicating if song is empty
    */
    bool isEmpty()
    {
        return musicTrackQueue.empty();
    }

    /** Method used by user get song list http request (userServices/SongListPage.cpp) to obtain a copy of song queue.
    * 
    *  @return A copy of the song queue
    */
    std::list<std::string> getSongQueue()
    {
        return musicTrackQueue;
    }

    /** Method used by user and supervisor to remove song from queue
    * 
    *  @param song: the song to remove
    */
    void removeSong(std::string song)
    {
        lockMutex();
        musicTrackQueue.remove(song);
        numberOfSongsDeleted++;
        unlockMutex();
    }

    /** Method used adminServices/GetStats.cpp to obtain the total number of songs deleted
    * 
    *  @return the total number of songs deleted
    */
    int getNumberOfSongsDeleted()
    {
        return numberOfSongsDeleted;
    }

    /** Method used by MediaPlayer thread to remove next song from queue and obtain its path for playback.
    * 
    *  @return The path of the next mp3 file
    */
    std::string removeAndReturnNextSong()
    {
        lockMutex();
        currentSong = musicTrackQueue.front();
        musicTrackQueue.pop_front();
        unlockMutex();
        return currentSong;
    }

    /** Method used by mp3 file uploading service for android users (userServices/UploaderPage.cpp) to add uploaded song in queue.
    * 
    *  @param filePath: The path of the mp3 file to add
    *  @param lengthInSeconds: The song's length in seconds
    */
    void addSongToQueue(std::string filePath, int lengthInSeconds)
    {
        lockMutex();
        musicTrackQueue.push_back(filePath);
        numberOfSongsUploaded++;
        totalUploadedLengthInSeconds += lengthInSeconds;
        unlockMutex();
    }

    /** Method used by mp3 file adminServices/GetStats.cpp to obtain the average length of uploaded songs
    * 
    *  @return average length of uploaded songs
    */
    int getAverageSongLength()
    {
        if (numberOfSongsUploaded == 0)
            return 0;
        return totalUploadedLengthInSeconds/numberOfSongsUploaded;
    }

    /** Method used by mp3 file adminServices/GetStats.cpp to obtain number of songs uploaded
    * 
    *  @return number of songs uploaded
    */
    int getNumberOfSongsUploaded()
    {
        return numberOfSongsUploaded;
    }

    /** Method used by supervisor to move songs in queue
    * 
    *  @param oldPos: the song's old position in queue
    *  @param newPos: the song's new position in queue
    */
    void moveSong(int oldPos, int newPos)
    {
        lockMutex();
        std::list<std::string>::iterator it = musicTrackQueue.begin();
        while (oldPos--)
            it++;
        std::string song = *it;
        musicTrackQueue.remove(song);
        std::list<std::string>::iterator it2 = musicTrackQueue.begin();
        while(newPos--)
            it2++;
        musicTrackQueue.insert(it2,song);
        unlockMutex();
    }

    /** Method used by main method of Server.cpp to setup audio device at server start.
    * 
    *  @return A boolean indicating if device was successfully setup
    */
    bool setupDevice()
    {
        return mp3Player.setUpDevice();
    }

    /** Method used to close device at server shutdown.
    */
    void close()
    {
        mp3Player.closeDevice();
        sem_close(&queueMutex);
    }

    /** Method used by MediaPlayer thread to play next song
    * 
    *  @param request The path of next song's mp3 file
    */
    void play(std::string filePath)
    {
        mp3Player.play(filePath);
    }

private:
    std::string currentSong;
    std::list<std::string> musicTrackQueue;
    sem_t queueMutex;
    AlsaMediaPlayer mp3Player;
    int numberOfSongsUploaded;
    int numberOfSongsDeleted;
    int totalUploadedLengthInSeconds;

    void initializeMutex()
    {
        if (sem_init(&queueMutex, 0, 1) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to initialize mutex for song queue.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void lockMutex()
    {
        if (sem_wait(&queueMutex) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to wait mutex for song queue.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void unlockMutex()
    {
        if (sem_post(&queueMutex) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to post mutex for song queue.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }
};

#endif