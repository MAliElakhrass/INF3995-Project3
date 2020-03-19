/*
SupBlockUserPage:   Libnavajo DynamicPage class that defines the supervisor service 
                    for changing password.

HTTP request : POST /superviseur/<usager>/changement_mdp
*/
#ifndef SUP_CHANGE_PASS
#define SUP_CHANGE_PASS

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

#define CHANGE_PASS_SUCCESS "You have successfully changed your password."

class SupChangePwdPage : public DynamicPage
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
            std::string logErrMsg = "Supervisor " + name + " attempted to change password without being logged in.";
            return logAndRespond(response, logErrMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }

        //check request method
        if (request->getRequestType() != POST_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to change password with non POST REST call.";
            return logAndRespond(response, logErrMsg, USE_POST_MSG, WRONG_METHOD_CODE);
        }

        //check request content
        //check request content
        std::string logErrMsg = "Supervisor " + name + " attempted to change password with wrong request content.";
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        json j;
        try
        {
            j = json::parse(content.begin(), content.end());
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        }
        if (j.find("ancien") == j.end() || j.find("nouveau") == j.end())
            return logAndRespond(response, logErrMsg, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);

        //Now compare entry
        json::iterator it = j.find("ancien");
        std::string oldPass = it.value();
        if (oldPass != ListContainer::getInstance()->getSupervisorList()->getSupervisorPassword(name))
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to change password with wrong password information.";
            return logAndRespond(response, logErrMsg, ID_INFO_PROBLEM_MSG, BAD_CONTENT_CODE);
        }

        //request is valid - change password
        it = j.find("nouveau");
        std::string newPass = it.value();
        ListContainer::getInstance()->getSupervisorList()->setSupervisorPassword(name, newPass);

        std::string logMsg = "Supervisor " + name + " has change his password.";
        return logAndRespond(response, logMsg, CHANGE_PASS_SUCCESS, OK_CODE);
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

#endif