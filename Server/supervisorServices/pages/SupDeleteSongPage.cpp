/*
DeleteSongPage: Libnavajo DynamicPage class that defines the supervisor service 
                for deleting a song in queue.

HTTP request : DELETE /superviseur/<usager>/chanson/<no>
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../ListContainer.cpp"
#include "../../constants.h"

using json = nlohmann::json;

class SupDeleteSongPage : public DynamicPage
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
        //make sure supervisor is logged in
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logMsg = "Supervisor " + name + " attempted to delete song without being logged in.";
            return logAndRespond(response, logMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }

        //check request validity
        if (request->getRequestType() != DELETE_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to delete song with non DELETE method call.";
            return logAndRespond(response, logErrMsg, USE_DELETE_MSG, WRONG_METHOD_CODE);
        }
            
        std::string song = findSong(ListContainer::getInstance()->getSongList()->getSongQueue());

        if (song == INEXISTANT_SONG)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to delete song a non existant song.";
            return logAndRespond(response, logErrMsg, INEXISTANT_SONG, ACCESS_REFUSED_CODE);
        }
        if (song == SONG_FILE_PROBLEM)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to delete song. There was a problem opening song's json file.";
            return logAndRespond(response, logErrMsg, SONG_FILE_PROBLEM, SERVER_ERROR_CODE);
        }

        //request was valid - delete song
        ListContainer::getInstance()->getSongList()->removeSong(song);
        ListContainer::getInstance()->getUserList()->removeSongFromUser(song);
        remove(song.c_str());
        remove(findJsonHeaderFile(song).c_str());

        std::string logMsg = "Supervisor " + name + " has successfully deleted song.";
        return logAndRespond(response, logMsg, DELETE_SUCCESS_MSG, OK_CODE);
    }

    /** Method used by SupervisorDynamicRepository to inform Page of request song number if valid delete request url.
    * 
    *  @param no: The song number
    */
    void setNo(int no) { songNo = no; }

    /** Method used by SupervisorDynamicRepository to inform Page of requester name if valid delete request url.
    * 
    *  @param supName: The requesting supervisor's name
    */
    void setName(std::string supName) { name = supName; }

private:
    int songNo;
    std::string name;

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return fromString(responseContent, response);
    }

    //this method finds the song path using list songs json header files
    std::string findSong(std::list<std::string> songQueue)
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
                return *it;
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