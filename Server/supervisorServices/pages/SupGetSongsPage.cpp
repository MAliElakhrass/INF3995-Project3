/*
SupGetSongsPage:    Libnavajo DynamicPage class that defines the supervisor service 
                    for obtaining song queue.

HTTP request : GET /superviseur/<usager>/file
*/
#include <nlohmann/json.hpp>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

class SupGetSongsPage : public DynamicPage
{
    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating UserDynamicRepository if the page was appropriate for request url
    */
    bool getPage(HttpRequest *request, HttpResponse *response)
    {
        std::string name = getSupervisorName((std::string) request->getUrl());

        //check if supervisor is logged in
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logMsg = "Supervisor " + name + " attempted to get song list without being logged in.";
            return logAndRespond(response, logMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }
        //check request method
        if (request->getRequestType() != GET_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to get song list with non GET REST call.";
            return logAndRespond(response, logErrMsg, USE_GET_MSG, WRONG_METHOD_CODE);
        }

        json responseContent;
        //insert current song in json for android app to get current song's lyrics
        responseContent["current song"];
        std::string currentSongPath = ListContainer::getInstance()->getSongList()->getCurrentSong();
        if (currentSongPath != NONE)
        {
            if (!findAndInsertInformation(currentSongPath, &responseContent, "current song"))
            {
                std::string logErrMsg = "There was an error opening file " + currentSongPath + ".";
                return logAndRespond(response, logErrMsg, JSON_FILE_PROBLEM_MSG, SERVER_ERROR_CODE);
            }
        }

        //insert song queue information
        responseContent["songs"];
        std::list<std::string> list = ListContainer::getInstance()->getSongList()->getSongQueue();
        
        //for each mp3 file in queue, get its json information file equivalent
        for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it)
        {
            if (!findAndInsertInformation((*it), &responseContent, "songs"))
            {
                std::string logErrMsg = "There was an error opening json headers for file " + (*it) + ".";
                return logAndRespond(response, logErrMsg, JSON_FILE_PROBLEM_MSG, SERVER_ERROR_CODE);
            }   
        }

        std::string logMsg = "Supervisor " + name + " has successfully obtained song list.";
        return logAndRespond(response, logMsg, responseContent.dump(JSON_FILE_INDENTATION), OK_CODE);
    }

    /** Method used to obtain supervisor name from request url
    * 
    *  @param url: The HTTP request url
    *  @return the supervisor name
    */
    std::string getSupervisorName(std::string url)
    {
        size_t firstOf = url.find_first_of("/") + 1;
        size_t lastOf = url.find_last_of("/");
        return url.substr(firstOf, lastOf - firstOf);
    }

    /** Method to find json header file and insert its content in responseContent json
    * 
    *  @param filePath: The path of the mp3 file
    *  @param responseContent: the json containing both the current song's information and the songs in queue's informations
    *  @param jsonKey: the key indicating if song is current or in queue
    * 
    *  @return A boolean indicating if the operation was successful
    */
    bool findAndInsertInformation(std::string filePath, json* responseContent, std::string jsonKey)
    {
        std::list<std::string> list = ListContainer::getInstance()->getSongList()->getSongQueue();
        std::shared_ptr<UserList> userList = ListContainer::getInstance()->getUserList();

        size_t lastof = filePath.find_last_of("/");
        std::string jsonFilePath = JSON_DIR + filePath.substr(lastof + 1) + ".json";
        std::ifstream file(jsonFilePath);
        if (!file)
            return false;
        json songJson;
        file >> songJson;
        file.close();

        //use proposedBy header inserted at upload to obtain additionnal user info
        std::string proposedBy = songJson["proposedBy"];
        int proposedByToken = userList->getUserToken(proposedBy);
        songJson["id"] = proposedByToken;
        songJson["IP"] = userList->getUserIp(proposedByToken);
        songJson["MAC"] = userList->getUserMAC(proposedByToken);
        (*responseContent)[jsonKey].push_back(songJson);

        return true;
    }

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return fromString(responseContent, response);
    }
};