/*
LoginPage:  Libnavajo DynamicPage class that defines the administrator service 
            for loging to his account.

HTTP request : POST /admin/login
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../AES.cpp"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

#define WRONG_METHOD_ADLOGIN "Administrator attempted to login with non POST REST call."
#define WRONG_CONTENT_ADLOGIN "Admin attempted to login with wrong request content."
#define WRONG_INFO_ADLOGIN "Admin entered wrong login information."
#define SUCCESS_ADLOGIN "Administrator has logged in."

class LoginPage : public DynamicPage
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
            return logError(response, WRONG_METHOD_ADLOGIN, WRONG_METHOD_CODE);

        //check request content
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logError(response, WRONG_CONTENT_ADLOGIN, BAD_CONTENT_CODE);
        std::string decypheredContent = aes.decrypt(content);
        if (decypheredContent == ENCRYPTION_ERROR)
            return logError(response, WRONG_CONTENT_ADLOGIN, BAD_CONTENT_CODE);
        json j;
        try
        {
            j = json::parse(decypheredContent);
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logError(response, WRONG_CONTENT_ADLOGIN, BAD_CONTENT_CODE);
        }
        
        if (j.find("usager") == j.end() || j.find("mot_de_passe") == j.end())
            return logError(response, WRONG_CONTENT_ADLOGIN, BAD_CONTENT_CODE);
       
        //check request info
        json::iterator it = j.find("usager");
        std::string name = it.value();
        it = j.find("mot_de_passe");
        std::string password = it.value();

        if (name != ListContainer::getInstance()->getAdminInfo()->getUserName() || 
            password != ListContainer::getInstance()->getAdminInfo()->getPassword())
            return logError(response, WRONG_INFO_ADLOGIN, ACCESS_REFUSED_CODE);

        //login successful
        NVJ_LOG->append(NVJ_INFO, SUCCESS_ADLOGIN);
        response->setHttpReturnCode(OK_CODE);
        
        aes.setTextLength(LOGIN_SUCCESS_MSG);
        response->setContent(aes.encrypt(LOGIN_SUCCESS_MSG), aes.getLength());
        return true;
	}

private:
    AES_CBC aes;

    bool logError(HttpResponse* response, std::string logMsg, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return false;
    }
};