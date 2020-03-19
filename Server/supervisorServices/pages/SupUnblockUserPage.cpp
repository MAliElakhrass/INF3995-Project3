/*
SupUnblockUserPage: Libnavajo DynamicPage class that defines the supervisor service 
                    for unblocking a user.

HTTP request : POST /superviseur/<usager>/debloquer
*/
#include <nlohmann/json.hpp>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

#define UNBLOCK_USER_SUCCESS "You have successfully unblocked user."

class SupUnblockUserPage : public DynamicPage
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

        //check if logged in
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to log off without being logged in.";
            return logAndRespond(response, logErrMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }

        //check request structure
        if (request->getRequestType() != POST_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to change song position with non POST method call.";
            return logAndRespond(response, logErrMsg, USE_POST_MSG, BAD_CONTENT_CODE);
        }
        
        //check request content
        std::string logErrMsg = "Supervisor " + name + " attempted to block user with bad request content.";
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        json requestcontent;
        try
        {
            requestcontent = json::parse(content.begin(), content.end());
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        }
        if (requestcontent.find("MAC") == requestcontent.end())
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        
        //request is valid - unblock user
        json::iterator it = requestcontent.find("MAC");
        std::string userMAC = it.value();
        
        if (!ListContainer::getInstance()->getBlacklist()->unblockUser(userMAC))
        {
            logErrMsg = "Supervisor " + name + " attempted to block user with unexistant MAC address.";
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        }

        std::string logMsg = "Supervisor " + name + " has successfully unblocked user with MAC addr " + userMAC + ".";
        return logAndRespond(response, logMsg, UNBLOCK_USER_SUCCESS, OK_CODE);
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