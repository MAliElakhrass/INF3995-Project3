/*
IdentificationPage : Libnavajo DynamicPage class that defines the user service 
                     for identification and obtaining token. The request for this service should contain
                     a json file specifying the the user's informations and
                     will return the token via HTTP response.
                     This page builds response only for a valid identification request.

HTTP request : POST /usager/identification
*/
#include <nlohmann/json.hpp>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"

using json = nlohmann::json;

#define UNKNOWN_USER_REQUEST_ERROR "An unknown user attempted to identitfy with wrong request structure."

class IdentificationPage : public DynamicPage
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
        //check request method
        if (request->getRequestType() != POST_METHOD)
            return logAndSignalError(response, WRONG_METHOD_CODE);

        //check request content
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logAndSignalError(response, BAD_CONTENT_CODE);
        json requestcontent;
        try
        {
            requestcontent = json::parse(content.begin(), content.end());
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logAndSignalError(response, BAD_CONTENT_CODE);
        }
        if (requestcontent.find("ip") == requestcontent.end() || requestcontent.find("MAC") == requestcontent.end() ||
            requestcontent.find("nom") == requestcontent.end())
            return logAndSignalError(response, BAD_CONTENT_CODE);
        
        //identification successful
        json::iterator it = requestcontent.find("MAC");
        std::string userMAC = it.value();
        it = requestcontent.find("nom");
        std::string userName = it.value();
        it = requestcontent.find("ip");
        std::string userIP = it.value();

        // check if user is already logged in
        json responseContent;
        std::string successMsg;
        int id = ListContainer::getInstance()->getUserList()->existUser(userName, userIP, userMAC);
        if (id < 0) {
            ListContainer::getInstance()->getUserList()->addUser(++lastId, userName, userIP, userMAC);
            
            responseContent["identificateur"] = lastId;
            responseContent["message"] = userName + " has been added.";

            successMsg = "Client " + userName + " (IP addr = " + userIP +
                                    ", MAC addr = "  + userMAC + ") has successfully identified himself to server.";
        } else {
            responseContent["identificateur"] = id;
            responseContent["message"] = userName + " was already identified.";
            successMsg = "Client " + userName + " (IP addr = " + userIP +
                                    "MAC addr = "  + userMAC + ") attempted to identify but was already identified.";
        }

        NVJ_LOG->append(NVJ_INFO, successMsg);
        response->setHttpReturnCode(OK_CODE);        

        return fromString(responseContent.dump(JSON_FILE_INDENTATION), response);
    }

    /** Method used by UserDynamicRepository to get last assigned Id to validate request urls with this new token
    * 
    *  @return last assigned id
    */
    int getLastId() { return lastId; }

  private:
    int lastId = 0;

    /** Method to log and signal error when identification request is invalid 
    * 
    *  @param response The HTTP response
    *  @param errMsg The log entry
    *  @param errorCode The HTTP response return code
    *  @return A boolean indicating Repository that identification request was denied
    */
    bool logAndSignalError(HttpResponse *response, int errorCode)
    {
        NVJ_LOG->append(NVJ_INFO, UNKNOWN_USER_REQUEST_ERROR);
        response->setHttpReturnCode(errorCode);
        return false;
    }
};