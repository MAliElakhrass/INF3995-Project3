/*
UserBlockedErrorPage:   Libnavajo DynamicPage class that informs user he has been blocked
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

class UserBlockedErrorPage : public DynamicPage
{
public:
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        std::string userName = ListContainer::getInstance()->getUserList()->getUserName(userId);
        std::string logErrMsg = "Client " + userName + " attempted to delete MP3 song when banned.";
        NVJ_LOG->append(NVJ_ERROR, logErrMsg);
        response->setHttpReturnCode(ACCESS_REFUSED_CODE);
        return fromString(BANNED_ERROR_MSG, response);
	}

    void setUserId(int id) { userId = id; }

private:
    int userId;
};