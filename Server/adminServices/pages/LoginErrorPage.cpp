/*
LoginErrorPage: Libnavajo DynamicPage class that builds response content on unsuccessful administrator login
*/
#ifndef LOGIN_ERROR
#define LOGIN_ERROR

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../AES.cpp"

#define ERROR_LOGIN "There was a problem with the information you entered."

class LoginErrorPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        aes.setTextLength(ERROR_LOGIN);
        response->setContent(aes.encrypt(ERROR_LOGIN),aes.getLength());
        return true;
	}
private:
    AES_CBC aes;
};

#endif