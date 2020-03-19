/*
SupGetSoundPage:    Libnavajo DynamicPage class that defines the supervisor service 
                    for obtaining volume.

HTTP request : GET /superviseur/<usager>/volume
*/
#include <nlohmann/json.hpp>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"
#include "../../audioPlayer/SoundControl.cpp"

using json = nlohmann::json;

class SupGetSoundPage : public DynamicPage
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

        //check if logged on
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to get volume without being logged in.";
            return logAndRespond(response, logErrMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }
        //check request method
        if (request->getRequestType() != GET_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to get volume with non GET REST call.";
            return logAndRespond(response, logErrMsg, USE_GET_MSG, WRONG_METHOD_CODE);
        }
        
        long volume = SoundControl::getInstance()->getVolume();
        bool isMuted = SoundControl::getInstance()->isSoundMuted();
        json responseContent;
        responseContent["volume"] = volume;
        responseContent["sourdine"] = isMuted;

        std::string logMsg = "Supervisor " + name + " has successfully obtained volume.";
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

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return fromString(responseContent, response);
    }
};