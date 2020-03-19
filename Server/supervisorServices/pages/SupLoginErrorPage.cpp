/*
LoginErrorPage: Libnavajo DynamicPage class that builds response content on unsuccessful supervisor login
*/
#ifndef LOGIN_ERROR_SUP
#define LOGIN_ERROR_SUP

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#define ERROR_LOGIN_SUP "There was a problem with the information you entered."

class SupLoginErrorPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        return fromString(ERROR_LOGIN_SUP, response);
	}
};

#endif