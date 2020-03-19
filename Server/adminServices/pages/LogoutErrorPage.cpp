/*
LogoutErrorPage:    Libnavajo DynamicPage class that builds the response content on 
                    unsuccessful logout attempt by administrator.

HTTP request : POST /admin/logout
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../AES.cpp"

#define WRONG_METHOD_LOGOUTERROR "Admin attempted to logout with non POST REST call."
#define LOGGED_IN_LOGOUTERROR "Admin attempted to logout without being looged in."

class LogoutErrorPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        if (request->getRequestType() != POST_METHOD)
            return logAndRespond(response, WRONG_METHOD_LOGOUTERROR, USE_POST_MSG, WRONG_METHOD_CODE);
        return logAndRespond(response, LOGGED_IN_LOGOUTERROR, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
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