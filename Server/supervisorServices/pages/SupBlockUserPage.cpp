/*
SupBlockUserPage:   Libnavajo DynamicPage class that defines the supervisor service 
                    for blocking a user.

HTTP request : POST /superviseur/<usager>/bloquer
*/
#include <nlohmann/json.hpp>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

#define USER_ALREADY_BLOCKED_MSG "User is already blocked."
#define BLOCK_USER_SUCCCESS_MSG "You have successfully blocked user."
#define BLOCK_UNEXISTANT_USER_MSG "There are no users who match those informations."

class SupBlockUserPage : public DynamicPage
{
    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating UserDynamicRepository if the page was appropriate for request url
    */
    bool getPage(HttpRequest *request, HttpResponse *response)
    {
        std::string requestUrl = (std::string) request->getUrl();
        std::string name = getSupervisorName(requestUrl);

        //check if logged in
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logMsg = "Supervisor " + name + " attempted to block user without being logged in.";
            return logAndRespond(response, logMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }

        //check request method     
        if (request->getRequestType() != POST_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to block user with non POST REST call.";
            return logAndRespond(response, logErrMsg, USE_POST_MSG, WRONG_METHOD_CODE);
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
        if (requestcontent.find("ip") == requestcontent.end() || requestcontent.find("MAC") == requestcontent.end() ||
             requestcontent.find("nom") == requestcontent.end())
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        
        //check if user is already blocked
        json::iterator it = requestcontent.find("nom");
        std::string userName = it.value();
        int userToken = ListContainer::getInstance()->getUserList()->getUserToken(userName);
        if (ListContainer::getInstance()->getBlacklist()->isUserBlocked(userToken))
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to block user that is already blocked.";
            return logAndRespond(response, logErrMsg, USER_ALREADY_BLOCKED_MSG, BAD_CONTENT_CODE);
        }
        
        //check if user exists
        it = requestcontent.find("MAC");
        std::string userMAC = it.value();
        it = requestcontent.find("ip");
        std::string userIP = it.value();

        if (ListContainer::getInstance()->getUserList()->existUser(userName, userIP, userMAC) == -1)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to block a user that does not exist.";
            return logAndRespond(response, logErrMsg, BLOCK_UNEXISTANT_USER_MSG, BAD_CONTENT_CODE);
        }

        //request valid - block user
        ListContainer::getInstance()->getBlacklist()->blockUser(userToken, userMAC, userIP, userName);
        std::string logMsg = "Supervisor " + name + " has successfully blocked user " + userName + ".";
        return logAndRespond(response, logMsg, BLOCK_USER_SUCCCESS_MSG, OK_CODE);
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