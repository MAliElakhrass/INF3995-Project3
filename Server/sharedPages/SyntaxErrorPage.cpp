/*
SyntaxErrorPage:    Libnavajo DynamicPage class that defines the response 
                    for a request using an inextants url.
*/
#ifndef SYNTAX_ERROR
#define SYNTAX_ERROR

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#define SYNTAX_ERROR_CONTENT "Error 400\nBad URL syntax"
#define SYNTAX_ERROR_LOG "Client application requested an non existent page."

class SyntaxErrorPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
		NVJ_LOG->append(NVJ_INFO, SYNTAX_ERROR_LOG);
		response->setHttpReturnCode(400);
		return fromString(SYNTAX_ERROR_CONTENT, response);
	}
};

#endif