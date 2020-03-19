/*
LogoutPage: Libnavajo DynamicPage class that builds the response content on 
            admin request while being logged off.

*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../AES.cpp"

#define UNAUTHORIZED_ADMIN_MSG "Unauthorized request for admin service."

class UnauthorizedPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        NVJ_LOG->append(NVJ_INFO, UNAUTHORIZED_ADMIN_MSG);
        response->setHttpReturnCode(UNAUTHENTIFIED_CODE);
        aes.setTextLength(LOGGED_OUT_MSG);
        response->setContent(aes.encrypt(LOGGED_OUT_MSG),aes.getLength());
        return true;
	}
private:
    AES_CBC aes;
};