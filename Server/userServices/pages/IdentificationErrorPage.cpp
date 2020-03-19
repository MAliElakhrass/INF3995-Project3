/*
IdentificationErrorPage : Libnavajo DynamicPage class that builds response content on an unsuccessful identification.
                          This page builds response only for invalid identification requests.

HTTP request : POST /usager/identification
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#define ID_ERROR_MSG "Error with identification request. Could not create token Id."

class IdentificationErrorPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        return fromString(ID_ERROR_MSG, response);
	}
};