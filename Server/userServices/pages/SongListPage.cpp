/*
ListPage : Libnavajo DynamicPage class that defines the user service 
           for obtaining songs in queue.

HTTP request : GET /usager/chanson/<Id>
*/
#include <nlohmann/json.hpp>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

class ListPage : public DynamicPage
{
    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating UserDynamicRepository if the page was appropriate for request url
    */
    bool getPage(HttpRequest *request, HttpResponse *response)
    {
        std::string userName = getUserName(request->getUrl());

        if (checkIfBanned(request->getUrl()))
        {
            std::string logErrMsg = "Client " + userName + " attempted to get MP3 list when banned.";
            return logAndRespond(response, ACCESS_REFUSED_CODE, logErrMsg, BANNED_ERROR_MSG);
        }

        if (request->getRequestType() != GET_METHOD)
        {
            std::string logErrMsg = "Client " + userName + " attempted to get MP3 list with non GET REST call.";
            return logAndRespond(response, WRONG_METHOD_CODE, logErrMsg, USE_GET_MSG);
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
                return logAndRespond(response, SERVER_ERROR_CODE, logErrMsg, JSON_FILE_PROBLEM_MSG);
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
                return logAndRespond(response, SERVER_ERROR_CODE, logErrMsg, JSON_FILE_PROBLEM_MSG);
            }   
        }

        std::string logSuccessMsg = "Client " + userName + " has successfully obtained song list.";
        return logAndRespond(response, OK_CODE, logSuccessMsg, responseContent.dump(JSON_FILE_INDENTATION));
    }

    /** Method used by in getPage to extract token from url and ask UserList object for user's name
    * 
    *  @param url The HTTP request url
    *  @return the token owner's registered name
    */
    std::string getUserName(std::string url)
    {
        size_t lastOf = url.find_last_of("/");
        std::string token = url.substr(lastOf + 1);
        return ListContainer::getInstance()->getUserList()->getUserName(stoi(token));
    }

    /** Method used to build response and log results
    * 
    *  @param response The HTTP response
    *  @param returnCode The HTTP response return code
    *  @param logMsg The HTTP response and log message
    *  @param responseContent the HTTP response content
    *  @return A boolean indicating Repository if the page was appropriate for request url
    */
    bool logAndRespond(HttpResponse *response, int returnCode, std::string logMsg, std::string responseContent)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return fromString(responseContent, response);
    }

    /** Method used to check if token owner is blacklisted
    * 
    *  @param url: The HTTP request url
    *  @return A boolean indicating if the user was banned.
    */
    bool checkIfBanned(std::string url)
    {
        size_t lastOf = url.find_last_of("/");
        int token = stoi(url.substr(lastOf + 1));
        return ListContainer::getInstance()->getBlacklist()->isUserBlocked(token);
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
        size_t lastof = filePath.find_last_of("/");
        std::string jsonFilePath = JSON_DIR + filePath.substr(lastof + 1) + ".json";
        std::ifstream file(jsonFilePath);
        if (!file)
            return false;
        json songJson;
        file >> songJson;
        file.close();
        (*responseContent)[jsonKey].push_back(songJson);

        return true;
    }
};