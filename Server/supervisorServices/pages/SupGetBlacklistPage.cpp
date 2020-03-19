/*
SupGetBlacklistPage:    Libnavajo DynamicPage class that defines the supervisor service 
                        for obtaining blacklist.

HTTP request : GET /superviseur/<usager>/listenoire
*/
#include <list>

#include <nlohmann/json.hpp>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

class SupGetBlacklistPage : public DynamicPage
{
    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating UserDynamicRepository if the page was appropriate for request url
    */
    bool getPage(HttpRequest *request, HttpResponse *response)
    {
        std::string name = getSupervisorName((std::string) request->getUrl());

        //check if logged on
        if (!ListContainer::getInstance()->getSupervisorList()->isSupervisorLoggedOn(name))
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to get black list without being logged in.";
            return logAndRespond(response, logErrMsg, LOGGED_OUT_MSG, UNAUTHENTIFIED_CODE);
        }
        //check request method
        if (request->getRequestType() != GET_METHOD)
        {
            std::string logErrMsg = "Supervisor " + name + " attempted to get black list with non GET REST call.";
            return logAndRespond(response, logErrMsg, USE_GET_MSG, WRONG_METHOD_CODE);
        }
        
        //get blacklist
        json responseContent;
        responseContent["bloques"];
        std::map<int, std::shared_ptr<BlockedUser>> blacklist = ListContainer::getInstance()->getBlacklist()->getBlacklist();

        for (std::map<int, std::shared_ptr<BlockedUser>>::iterator it = blacklist.begin(); it != blacklist.end(); ++it)
        {
            json tempJson;
            tempJson["nom"] = it->second->getName();
            tempJson["ip"] = it->second->getIp();
            tempJson["MAC"] = it->second->getMAC();
            responseContent["bloques"].push_back(tempJson);
        }

        responseContent["users"];
        std::map<int, std::shared_ptr<Client>> users = ListContainer::getInstance()->getUserList()->getUsers();
        for (std::map<int, std::shared_ptr<Client>>::iterator it = users.begin(); it != users.end(); ++it)
        {
            json tempJson;
            tempJson["nom"] = it->second->getClientName();
            tempJson["ip"] = it->second->getIpAddr();
            tempJson["MAC"] = it->second->getMAC();
            responseContent["users"].push_back(tempJson);
        }

        std::string logMsg = "Supervisor " + name + " has successfully obtained blacklist.";
        return logAndRespond(response, logMsg, responseContent.dump(JSON_FILE_INDENTATION), OK_CODE);
    }

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