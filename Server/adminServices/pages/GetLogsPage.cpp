/*
GetLogsPage : Libnavajo DynamicPage class that defines the administrator service 
              of obtaining server logs. The request for this service should contain
              a json file specifying the logs to be returned via HTTP response.

HTTP request : POST /admin/logs
*/
#include <fstream>
#include <limits>
#include <string>
#include <iostream>

#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"
#include <nlohmann/json.hpp>

#include "../../constants.h"
#include "../AES.cpp"

#define WRONG_METHOD_MSG_GETLOGS "Administrator attempted to get logs with wrong request method."
#define WRONG_CONTENT_MSG_GETLOGS "Administrator attempted to get logs with wrong json content."
#define LOG_FILE_ERROR_MSG "There was an error opening log file for administrator."
#define SUCCESS_MSG_GETLOGS "Administrator asked for server logs."

using json = nlohmann::json;

class GetLogsPage : public DynamicPage
{
    AES_CBC aes;
    
    /** Method used to build response and log results
    * 
    *  @param response The HTTP response
    *  @param returnCode The HTTP response return code
    *  @param logMsg The HTTP response and log message
    *  @param responseContent the HTTP response content
    *  @return A boolean indicating Repository if the page was appropriate for request url
    */
    bool logAndRespond(HttpResponse *response, int returnCode, std::string logMsg, std::string responseContent)
    {
        NVJ_LOG->append(NVJ_INFO, logMsg);
        response->setHttpReturnCode(returnCode);
        aes.setTextLength(responseContent);
        response->setContent(aes.encrypt(responseContent),aes.getLength());
        return true;
    }

    /** Method used to get log number of 20th to last log entry.
    * 
    *  @param logFile The log file
    *  @return The log number of 20th to last log entry
    */
    int getNo(std::ifstream& logFile)
    {
        std::string line;
        int numberOfLines = 0;
        //count number of lines in log file
        while(!logFile.eof()) {
	        std::getline(logFile, line);
	        numberOfLines++;	
        }
        if (numberOfLines > 20)
            numberOfLines -= 20;
        else
            numberOfLines = 1;

        return numberOfLines;
    }

    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating AdminDynamicRepository if the page was appropriate for request url
    */
	bool getPage(HttpRequest *request, HttpResponse *response)
	{
        //check request method validity
        if (request->getRequestType() != POST_METHOD)
            return logAndRespond(response, WRONG_METHOD_CODE, WRONG_METHOD_MSG_GETLOGS, USE_POST_MSG);

        //check request content validity
        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
            return logAndRespond(response, BAD_CONTENT_CODE, WRONG_CONTENT_MSG_GETLOGS, WRONG_CONTENT_MSG_GETLOGS);
        std::string decypheredContent = aes.decrypt(content);
        if (decypheredContent == ENCRYPTION_ERROR)
            return logAndRespond(response, BAD_CONTENT_CODE, WRONG_CONTENT_MSG_GETLOGS, WRONG_CONTENT_MSG_GETLOGS);

        int pos = decypheredContent.find('}');
        if (pos > 0)
            decypheredContent = decypheredContent.substr(0, pos + 1);

        json requestcontent;
        try
        {
            requestcontent = json::parse(decypheredContent);
        }
        catch(nlohmann::detail::parse_error e)
        {
            return logAndRespond(response, BAD_CONTENT_CODE, WRONG_CONTENT_MSG_GETLOGS, WRONG_CONTENT_MSG_GETLOGS);
        }
        
        if (requestcontent.find("dernier") == requestcontent.end())
            return logAndRespond(response, BAD_CONTENT_CODE, WRONG_CONTENT_MSG_GETLOGS, WRONG_CONTENT_MSG_GETLOGS);

        json::iterator it = requestcontent.find("dernier");
        int no = it.value();
        if (no < 0)
            return logAndRespond(response, BAD_CONTENT_CODE, WRONG_CONTENT_MSG_GETLOGS, WRONG_CONTENT_MSG_GETLOGS);

        //request is valid
        std::ifstream logFile(LOG_FILE_PATH);
        if (!logFile)
            return logAndRespond(response, SERVER_ERROR_CODE, LOG_FILE_ERROR_MSG, LOG_FILE_ERROR_MSG);
            
        
        //if no == 0, get 20 last logs.
        if (no == 0)
        {
            no = getNo(logFile);
            logFile.clear();
        }
            
        /*
        This next loop was taken from stack overflow to go to log entry "no" in file
        https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
        */
        logFile.seekg(0, std::ios::beg);
        for(int i=0; i < no - 1; ++i){
            logFile.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }

        json responseContent;
        json tempJson;
        std::string line;
        responseContent["information"];

        while (true)
        {
            std::getline(logFile, line);
            if (logFile.eof())
                break;

            tempJson["no"] = std::to_string(no++);
            tempJson["info"] = line;
            responseContent["information"].push_back(tempJson);
        }

        logFile.close();

        return logAndRespond(response, OK_CODE, SUCCESS_MSG_GETLOGS, responseContent.dump(JSON_FILE_INDENTATION));
	}
};