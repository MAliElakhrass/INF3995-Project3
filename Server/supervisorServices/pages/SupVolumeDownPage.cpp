/*
SupVolumeDownPage:      Libnavajo DynamicPage class that defines the supervisor service 
                        for turning volume down.

HTTP request : POST /superviseur/<usager>/volume/diminuer/<pc>
*/
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

class SupVolumeDownPage : public DynamicPage
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

        //check if logged on
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to turn volume down without being logged in.";
            return logAndRespond(response, logErrMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }
        //check request method
        if (request->getRequestType() != POST_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to turn volume down with non POST REST call.";
            return logAndRespond(response, logErrMsg, USE_POST_MSG, WRONG_METHOD_CODE);
        }
        
        SoundControl::getInstance()->lowerVolume(pc);

        std::string logMsg = "Supervisor " + name + " has successfully turnED volume down.";
        return logAndRespond(response, logMsg, std::to_string(pc), OK_CODE);
    }

    /** Method used by Repository to set pc before calling getPage
    * 
    *  @param input: The new value of pc
    */
    void setPc(int input) { pc = input; }

    /** Method used by SupervisorDynamicRepository to inform Page of requester name if valid delete request url.
    * 
    *  @param supName: The requesting supervisor's name
    */
    void setName(std::string supName) { name = supName; }

private:
    int pc;
    std::string name;

    /** Method used to obtain supervisor name from request url
    * 
    *  @param url: The HTTP request url
    *  @return the supervisor name
    */
    std::string getSupervisorName(std::string url)
    {
        size_t firstOf = url.find_first_of("/") + 1;
        size_t lastOf = url.find_last_of("/");
        return url.substr(firstOf, lastOf - firstOf);
    }

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        return fromString(responseContent, response);
    }
};