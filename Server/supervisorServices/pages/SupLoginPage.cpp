/*
SupLoginPage:   Libnavajo DynamicPage class that defines the supervisor service 
                for logging on.

HTTP request : POST /superviseur/login
*/
#ifndef SUP_LOGIN
#define SUP_LOGIN

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

class SupLoginPage : public DynamicPage
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
        //check request method
        if (request->getRequestType() != POST_METHOD)
            return logError(response, WRONG_CONTENT_MSG, WRONG_METHOD_CODE);

        //check request content
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logError(response, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        json j;
        try
        {
            j = json::parse(content.begin(), content.end());
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logError(response, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        }
        if (j.find("usager") == j.end() || j.find("mot_de_passe") == j.end())
            return logError(response, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);

        //check information entered
        json::iterator it = j.find("usager");
        std::string name = it.value();
        it = j.find("mot_de_passe");
        std::string password = it.value();

        std::shared_ptr<SupervisorList> supList = ListContainer::getInstance()->getSupervisorList();
        if (!supList->supervisorExists(name) || password != supList->getSupervisorPassword(name))
            return logError(response, WRONG_LOGIN_INFO, BAD_CONTENT_CODE);

        //login successful
        supList->loginSupervisor(name);
        NVJ_LOG->append(NVJ_INFO, "Supervisor " + name + " has logged in.");
        response->setHttpReturnCode(OK_CODE);
        return fromString(LOGIN_SUCCESS_MSG, response);
	}

private:

    bool logError(HttpResponse* response, std::string logErrMsg, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logErrMsg);
        response->setHttpReturnCode(returnCode);
        return false;
    }
};

#endif