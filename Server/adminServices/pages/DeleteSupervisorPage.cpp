/*
DeleteSupervisorPage:   Libnavajo DynamicPage class that defines the administrator service 
                        for deleting a supervisor account.

HTTP request : POST admin/suppressioncompte
*/
#include <memory>

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../../ListContainer.cpp"
#include "../../supervisorServices/SupervisorDynamicRepository.cpp"
#include "../AES.cpp"

using json = nlohmann::json;

#define WRONG_METHOD_DELETESUP "Admin attempted to delete supervisor account using a non POST method."
#define WRONG_CONTENT_DELETESUP "Admin attempted to delete  supervisor account using bad request content."
#define INEXISTANT_DELETESUP "This supervisor does not exist."
#define INEXISTANT_DELETESUP_LOG "Admin attempted to delete a non existant supervisor."
#define SUCCESS_DELETESUP "You have successfully deleted a supervisor."

class DeleteSupervisorPage : public DynamicPage
{
public:

    /** Method used by AdminDynamicRepository to associate the supervisor repository with DeleteSupervisorPage
    * 
    *  @param repo: The SupervisorDynamicRepository
    */
    void setRepo(std::shared_ptr<SupervisorDynamicRepository> repo)
    {
        supRepo = repo;
    }

private:
    std::shared_ptr<SupervisorDynamicRepository> supRepo;
    AES_CBC aes;

    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating UserDynamicRepository if the page was appropriate for request url
    */
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        //check request validity
        if (request->getRequestType() != POST_METHOD)
            return logAndRespond(response, WRONG_METHOD_DELETESUP, USE_POST_MSG, WRONG_METHOD_CODE);
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logAndRespond(response, WRONG_CONTENT_DELETESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        std::string decypheredContent = aes.decrypt(content);
        if (decypheredContent == ENCRYPTION_ERROR)
            return logAndRespond(response, WRONG_CONTENT_DELETESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        
        int pos = decypheredContent.find('}');
        if (pos > 0)
            decypheredContent = decypheredContent.substr(0, pos + 1);
        
        json requestContent;
        try
        {
            requestContent = json::parse(decypheredContent);
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logAndRespond(response, WRONG_CONTENT_DELETESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        }
        if (requestContent.find("usager") == requestContent.end())
            return logAndRespond(response, WRONG_CONTENT_DELETESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);

        //check if supervisor exists
        json::iterator it = requestContent.find("usager");
        std::string name = it.value();
        if (!ListContainer::getInstance()->getSupervisorList()->removeSupervisor(name))
            return logAndRespond(response, INEXISTANT_DELETESUP_LOG, INEXISTANT_DELETESUP, BAD_CONTENT_CODE);

        //request is valid - remove services for this supervisor
        supRepo->removeURLs(name);
        std::string logMsg = "Admin as successfully deleted supervisor " + name + ").";
        return logAndRespond(response, logMsg, SUCCESS_DELETESUP, OK_CODE);
	}

    bool logAndRespond(HttpResponse* response, std::string logMsg, std::string responseContent, int returnCode)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        aes.setTextLength(responseContent);
        response->setContent(aes.encrypt(responseContent),aes.getLength());
        return true;
    }
};