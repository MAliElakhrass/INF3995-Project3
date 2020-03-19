/*
GetStatsPage:   Libnavajo DynamicPage class that defines the administrator service 
                for obtaining basic system stats.

HTTP request : GET /admin/statistiques
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../../ListContainer.cpp"
#include "../AES.cpp"

using json = nlohmann::json;

#define WRONG_METHOD_GETSTATS_LOG "Administrator attempted to obtain stats with wrong request method."
#define GET_STATS_SUCCESS_LOG "Administrator has successfully obtained system statistics."

class GetStatsPage : public DynamicPage
{
    AES_CBC aes;

    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating UserDynamicRepository if the page was appropriate for request url
    */
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        //check request validity
        if (request->getRequestType() != GET_METHOD)
            return logAndRespond(response, WRONG_METHOD_GETSTATS_LOG, USE_GET_MSG, WRONG_METHOD_CODE);
        
        //request is valid - send statistics
        json responseContent;
        responseContent["chansons"] = ListContainer::getInstance()->getSongList()->getNumberOfSongsUploaded();
        responseContent["utilisateurs"] = ListContainer::getInstance()->getUserList()->getNumberOfUsers();
        responseContent["elemines"] = ListContainer::getInstance()->getSongList()->getNumberOfSongsDeleted();
        int averageLengthInSecond = ListContainer::getInstance()->getSongList()->getAverageSongLength();
        int seconds = averageLengthInSecond % 60;
        int minutes = (averageLengthInSecond - seconds) / 60;
        std::string average = std::to_string(minutes) + " : " + std::to_string(seconds);
        responseContent["temps"] = average;

        return logAndRespond(response, GET_STATS_SUCCESS_LOG, responseContent.dump(JSON_FILE_INDENTATION), OK_CODE);
	}

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        aes.setTextLength(responseContent);
        response->setContent(aes.encrypt(responseContent),aes.getLength());
        return true;
    }
};