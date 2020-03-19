/*
SupLogoutPage:      Libnavajo DynamicPage class that defines the supervisor service 
                    for logging out.

HTTP request : POST /superviseur/<usager>/logout
*/
#ifndef SUP_LOGOUT
#define SUP_LOGOUT

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

class SupLogoutPage : public DynamicPage
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
        std::string name = getSupervisorName((std::string) request->getUrl());
        //check request method
        if (request->getRequestType() != POST_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to logout with non POST REST call.";
            return logAndRespond(response, logErrMsg, USE_POST_MSG, WRONG_METHOD_CODE);
        }

        //check if already logged in
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to log off without being logged in.";
            return logAndRespond(response, logErrMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }

        //logout successful
        ListContainer::getInstance()->getSupervisorList()->logoutSupervisor(name);
        std::string logMsg = "Supervisor " + name + " has logged off.";
        return logAndRespond(response, logMsg, LOGOUT_SUCCESS_MSG, OK_CODE);
	}

private:

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