/*
ChangeAdminPwdPage:  Libnavajo DynamicPage class that defines the administrator service 
                     for changing admin account password.

HTTP request : POST /admin/changement_mdp
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../AES.cpp"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

#define WRONG_METHOD_ADCHANGE "Administrator attempted to change password with non POST REST call."
#define WRONG_CONTENT_ADCHANGE "Admin attempted to change password with wrong request content."
#define WRONG_INFO_ADCHANGE_LOG "Admin entered wrong old password when trying to change pass."
#define WRONG_INFO_ADCHANGE "You entered the wrong old password."
#define SUCCESS_ADCHANGE "Administrator has successfully changed his password."

class ChangeAdminPwdPage : public DynamicPage
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
            return logAndRespond(response, WRONG_METHOD_ADCHANGE, USE_POST_MSG, WRONG_METHOD_CODE);

        //check request content
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logAndRespond(response, WRONG_CONTENT_ADCHANGE, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        std::string decypheredContent = aes.decrypt(content);
        if (decypheredContent == ENCRYPTION_ERROR)
            return logAndRespond(response, WRONG_CONTENT_ADCHANGE, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        json j;
        try
        {
            j = json::parse(decypheredContent);
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logAndRespond(response, WRONG_CONTENT_ADCHANGE, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
        }
        
        if (j.find("ancien") == j.end() || j.find("nouveau") == j.end())
            return logAndRespond(response, WRONG_CONTENT_ADCHANGE, WRONG_CONTENT_SUP_MSG, BAD_CONTENT_CODE);
       
        //check request info
        json::iterator it = j.find("ancien");
        std::string old = it.value();
        it = j.find("nouveau");
        std::string newPass = it.value();

        if (old != ListContainer::getInstance()->getAdminInfo()->getPassword())
            return logAndRespond(response, WRONG_INFO_ADCHANGE_LOG, WRONG_INFO_ADCHANGE, BAD_CONTENT_CODE);

        //success
        ListContainer::getInstance()->getAdminInfo()->setPassword(newPass);
        return logAndRespond(response, SUCCESS_ADCHANGE, SUCCESS_ADCHANGE, OK_CODE);
	}

private:
    AES_CBC aes;

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        aes.setTextLength(responseContent);
        response->setContent(aes.encrypt(responseContent),aes.getLength());
        return true;
    }
};