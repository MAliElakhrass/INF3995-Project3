/*
SupUnmutePage:      Libnavajo DynamicPage class that defines the supervisor service 
                    for unmuting sound.

HTTP request : POST /superviseur/<usager>/volume/sourdine/desactiver
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

class SupUnmutePage : public DynamicPage
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
            std::string logErrMsg = "Supervisor " + name + " attempted to unmute sound without being logged in.";
            return logAndRespond(response, logErrMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }
        //check request method
        if (request->getRequestType() != POST_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to unmute sound with non POST REST call.";
            return logAndRespond(response, logErrMsg, USE_POST_MSG, WRONG_METHOD_CODE);
        }
        
        SoundControl::getInstance()->unmuteVolume();

        std::string logMsg = "Supervisor " + name + " has successfully unmuted sound.";
        return logAndRespond(response, logMsg, "unmuted", OK_CODE);
    }

    /** Method used to obtain supervisor name from request url
    * 
    *  @param url: The HTTP request url
    *  @return the supervisor name
    */
    std::string getSupervisorName(std::string url)
    {
        size_t firstOf = url.find_first_of("/") + 1;
        url = url.substr(firstOf);
        firstOf = url.find_first_of("/");
        return url.substr(0, firstOf);
    }

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return fromString(responseContent, response);
    }
};