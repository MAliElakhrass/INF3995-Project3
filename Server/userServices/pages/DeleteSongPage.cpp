/*
DeleteSongPage: Libnavajo DynamicPage class that defines the user service 
                for deleting a user's own song in queue.

HTTP request : DELETE /usager/chanson/<Id>/<no>
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

#define WRONG_SONG_OWNER "Song does not belong to requester"

class DeleteSongPage : public DynamicPage
{
public:

    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating UserDynamicRepository if the page was appropriate for request url
    */
    bool getPage(HttpRequest *request, HttpResponse *response)
    {
        std::string name = ListContainer::getInstance()->getUserList()->getUserName(token);

        //check request validity
        if (request->getRequestType() != DELETE_METHOD)
        {
            std::string logMsg = "User " + name + " attempted to delete a song with non DELETE method.";
            return logAndRespond(response, logMsg, USE_DELETE_MSG, WRONG_METHOD_CODE);
        }
            
        std::string song = findSong(ListContainer::getInstance()->getSongList()->getSongQueue(), name);

        if (song == INEXISTANT_SONG)
        {
            std::string logErrMsg = "User " + name + " attempted to delete a non existant song.";
            return logAndRespond(response, logErrMsg, INEXISTANT_SONG, ACCESS_REFUSED_CODE);
        }
        if (song == WRONG_SONG_OWNER)
        {
            std::string logErrMsg = "Client " + name + " attempted to delete someone else's song.";
            return logAndRespond(response, logErrMsg, WRONG_SONG_OWNER, ACCESS_REFUSED_CODE);
        }
        if (song == SONG_FILE_PROBLEM)
        {
            std::string logErrMsg = "Client " + name + " attempted to delete song. There was a problem opening song's json file.";
            return logAndRespond(response, logErrMsg, SONG_FILE_PROBLEM, SERVER_ERROR_CODE);
        }

        //request is valid - deleting song
        ListContainer::getInstance()->getSongList()->removeSong(song);
        ListContainer::getInstance()->getUserList()->removeSongFromUser(song);
        remove(song.c_str());
        remove(findJsonHeaderFile(song).c_str());

        std::string logMsg = "User " + name + " has successfully deleted song.";
        return logAndRespond(response, logMsg, DELETE_SUCCESS_MSG, OK_CODE);
    }

    /** Method used by UserDynamicRepository to inform Page of request song number if valid delete request url.
    * 
    *  @param no: The song number
    */
    void setNo(int no) { songNo = no; }

    /** Method used by UserDynamicRepository to inform Page of requester token if valid delete request url.
    * 
    *  @param id: The requester's token
    */
    void setId(int id) { token = id; }

private:
    int songNo;
    int token;

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return fromString(responseContent, response);
    }

    //this method finds the song path using list songs json header files
    std::string findSong(std::list<std::string> songQueue, std::string requesterName)
    {
        std::list<std::string>::iterator it;
        for (it = songQueue.begin(); it != songQueue.end(); ++it)
        {
            std::string jsonFilePath = findJsonHeaderFile(*it);
            std::ifstream file(jsonFilePath);
            if (!file)
                return SONG_FILE_PROBLEM;
            json tempJson;
            file >> tempJson;
            file.close();
            if (tempJson["No"] == songNo)
            {
                if (tempJson["proposedBy"] == requesterName)
                    return *it;
                return WRONG_SONG_OWNER;
            }
        }
        return INEXISTANT_SONG;
    }

    //method to find mp3 file's corresponding json file path
    std::string findJsonHeaderFile(std::string mp3FilePath)
    {
        size_t lastof = mp3FilePath.find_last_of("/");
        return JSON_DIR + mp3FilePath.substr(lastof + 1) + JSON_FILE_EXT;
    }
};