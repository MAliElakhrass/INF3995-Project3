/*
LogoutPage: Libnavajo DynamicPage class that builds the response content on 
            successful logout attempt by administrator.

HTTP request : POST /admin/logout
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../AES.cpp"

#define LOG_ADMIN_LOGOUT "Administrator has logged off."

class LogoutPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        NVJ_LOG->append(NVJ_INFO, LOG_ADMIN_LOGOUT);
        response->setHttpReturnCode(OK_CODE);
        aes.setTextLength(LOGOUT_SUCCESS_MSG);
        response->setContent(aes.encrypt(LOGOUT_SUCCESS_MSG),aes.getLength());
        return true;
	}
private:
    AES_CBC aes;
};