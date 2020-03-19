/*
CreateSupervisorPage:   Libnavajo DynamicPage class that defines the administrator service 
                        for creating a supervisor account.

HTTP request : POST admin/creationcompte
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

#define WRONG_METHOD_CREATESUP "admin attempted to create supervisor with non POST REST call."
#define WRONG_CONTENT_CREATESUP "admin attempted to create supervisor with no request content."
#define SUCCES_MSG_CREATESUP "You have successfully added a new supervisor."

class CreateSupervisorPage : public DynamicPage
{
public:
    
    /** Method used by AdminDynamicRepository to associate the supervisor repository with CreateSupervisorPage
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
        //check request method validity
        if (request->getRequestType() != POST_METHOD)
            return logAndRespond(response, WRONG_METHOD_CREATESUP, USE_POST_MSG, WRONG_METHOD_CODE);

        //check request content validity
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logAndRespond(response, WRONG_CONTENT_CREATESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        std::string decypheredContent = aes.decrypt(content);
        if (decypheredContent == ENCRYPTION_ERROR)
            return logAndRespond(response, WRONG_CONTENT_CREATESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        json j;
        try
        {
            j = json::parse(decypheredContent);
        }
        catch (nlohmann::detail::parse_error e)
        {
            return logAndRespond(response, WRONG_CONTENT_CREATESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);
        }
        if (j.find("usager") == j.end() || j.find("mot_de_passe") == j.end())
            return logAndRespond(response, WRONG_CONTENT_CREATESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);

        //check if supervisor already exists
        json::iterator it = j.find("usager");
        std::string name = it.value();
        if (ListContainer::getInstance()->getSupervisorList()->supervisorExists(name))
            return logAndRespond(response, WRONG_CONTENT_CREATESUP, WRONG_CONTENT_MSG, BAD_CONTENT_CODE);

        //request is valid - add new supervisor
        it = j.find("mot_de_passe");
        std::string password = it.value();
        
        ListContainer::getInstance()->getSupervisorList()->addSupervisor(name,password);
        supRepo->addURLs(name); //associate new requests url based on new supervisor to supervisor services

        std::string logMsg = "Admin as added a new supervisor (name = " + name + ").";

        return logAndRespond(response, logMsg, SUCCES_MSG_CREATESUP, OK_CODE);
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