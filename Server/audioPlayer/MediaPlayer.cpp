/*
MediaPlayer : Thread responsible for checking song queue, activate playback of next song in queue,
              remove played songs and update user's uploaded songs in queue information.
*/
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../constants.h"
#include "../ListContainer.cpp"

using json = nlohmann::json;

void* waitAndPlay(void* unused)
{
    std::shared_ptr<UserList> userList = ListContainer::getInstance()->getUserList();
    std::shared_ptr<SongList> songList = ListContainer::getInstance()->getSongList();
    while(1)
    {
        //block  when queue empty
        while(songList->isEmpty());

        std::string filePath = songList->removeAndReturnNextSong();
        userList->removeSongFromUser(filePath);

        //logging
        size_t index1 = std::string(UPLOAD_DIR).length();
        size_t index2 = filePath.length();
        std::string jsonFilePath = JSON_DIR + filePath.substr(index1, index2 - index1) + JSON_FILE_EXT;
        std::ifstream jsonFile(jsonFilePath.c_str());
        if (!jsonFile)
        {
            std::string logErrMsg = "Problem reading from json file " + jsonFilePath + " in MediaPlayer thread.";
            NVJ_LOG->append(NVJ_INFO, logErrMsg);
        }          
        else
        {
            json j;
            jsonFile >> j;
            jsonFile.close();
            std::string songTitle = j["title"];
            std::string logMsg = "Starting playback of song " + songTitle + ".";
            NVJ_LOG->append(NVJ_INFO, logMsg);
        }

        songList->play(filePath);
        songList->noCurrentSong();

        //delete mp3 file
        if (remove(filePath.c_str()) != 0)
            NVJ_LOG->append(NVJ_WARNING, "There was an error removing mp3 file " + filePath + ".");
        //delete json file
        size_t pathIndex = filePath.find_last_of("/");
        filePath = JSON_DIR + filePath.substr(pathIndex + 1) + ".json";
        if (remove(filePath.c_str()) != 0)
            NVJ_LOG->append(NVJ_WARNING, "There was an error removing json file " + filePath + ".");
    }
}