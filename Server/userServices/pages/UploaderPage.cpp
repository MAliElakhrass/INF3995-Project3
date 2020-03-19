/*
MP3Uploader : Libnavajo DynamicPage class that defines the user service 
              for uploading mp3 files to server. The request for this service should contain
              a json file containing mp3 files encoded in base64.
              This page decodes the base64 coded information, verifies mp3 validity, generates json header file
              and inserts song in song queue.

HTTP request : POST /usager/chanson/<id>
*/
#include <unistd.h>

#include <nlohmann/json.hpp>
#include <taglib/fileref.h>
#include "libnavajo/libnavajo.hh"
#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "../../constants.h"
#include "../../ListContainer.cpp"
#include "../Base64Decoder.cpp"

#define BASE_FILE_NAME "song"
#define MAX_SONGS_USER 5
#define MAX_SONGS 10
#define UPLOAD_PROBLEM_MSG "There was a problem uploading file.\n"
#define INVALID_MP3_MSG "File is not either not an mp3 or is too big.\n"
#define FILE_TOO_BIG_MSG "File was too big for upload.\n"
#define UPLOAD_SUCCESS_MSG "File was successfully uploaded.\n"
#define MAX_SONG_MSG "Could not upload the other files as user has reached a maximum of five songs in queue."
#define NO_PROBLEM_CODE -1

using json = nlohmann::json;

class MP3Uploader : public DynamicPage
{
    Base64Decoder decoder;
    int songNo = 1;

    /** Method used to get token from request url
    * 
    *  @param url: The HTTP request url
    *  @return the uploader's token
    */
    int findSubmitter(std::string url)
    {
        size_t lastof = url.find_last_of("/");
        std::string idString = url.substr(lastof + 1);
        int id = stoi(idString);
        return id;
    }

    /** Method used to check if token owner is blacklisted
    * 
    *  @param url: The HTTP request url
    *  @return A boolean indicating if the user was banned.
    */
    bool checkIfBanned(std::string url)
    {
        size_t lastOf = url.find_last_of("/");
        int token = stoi(url.substr(lastOf + 1));
        return ListContainer::getInstance()->getBlacklist()->isUserBlocked(token);
    }

    /** Method used to verify uploaded file is mp3 and generate json header file
    * 
    *  @param filePath: The uploaded file path
    *  @param submittedBy: The registered name of the uploader for the "proposedBy" header
    *  @return A return code indicating the error if there was one, or -1 if there were no errors.
    */
    int generateMp3HeaderFile(std::string filePath, std::string submittedBy)
    {   
        try 
        {
            TagLib::FileRef f(filePath.c_str());

            if (f.audioProperties()->lengthInMilliseconds() == 0)
                return INVALID_MP3_CODE; //not an mp3 file

            json file;
            if ((file["title"] = (f.tag()->title()).to8Bit()) == "")
                file["title"] = "Unknown title";
            if ((file["artist"] = (f.tag()->artist()).to8Bit()) == "")
                file["artist"] = "Unknown artist";
            if ((file["album"] = (f.tag()->album()).to8Bit()) == "")
                file["album"] = "Unknown album";
            int lengthSec = f.audioProperties()->lengthInSeconds();
            file["minutes"] = (lengthSec / 60) % 60;
            file["seconds"] = lengthSec % 60;
            file["bitrate"] = f.audioProperties()->bitrate();
            file["sampleRate"] = f.audioProperties()->sampleRate();
            file["channels"] = f.audioProperties()->channels();
            file["proposedBy"] = submittedBy;
            file["No"] = songNo++;
            size_t index1 = std::string(UPLOAD_DIR).length();
            size_t index2 = filePath.length();
            std::string dstFileName = JSON_DIR + filePath.substr(index1, index2 - index1) + JSON_FILE_EXT;
            std::ofstream dst(dstFileName.c_str());
            if (!dst)
            {
                NVJ_LOG->append(NVJ_INFO, "Problem writing json file " + dstFileName);
                return SERVER_ERROR_CODE;
            }
            else
            {
                dst << file.dump(JSON_FILE_INDENTATION);
                dst.close();
            }
        }
        catch (nlohmann::detail::type_error e)
        {
            //This error will be thrown if there was a timeout on upload (file was too big!)
            return SONG_LIMIT_CODE;
        }

        return NO_PROBLEM_CODE;
    }

    /** Method used to return the title of the mp3
    * 
    *  @param filePath: The uploaded file path
    *  @return A string indicating the title
    */
    std::string getTitle(std::string filePath) {
        TagLib::FileRef f(filePath.c_str());

        if ((f.tag()->title()).to8Bit() == "")
            return "Unknown title";

        return f.tag()->title().to8Bit();
    }

    /** Method used to return the length of the mp3 song in seconds
    * 
    *  @param filePath: The uploaded file path
    *  @return A integer indicating the song's length in seconds
    */
    int getSongLength(std::string filePath)
    {
        TagLib::FileRef f(filePath.c_str());
        return f.audioProperties()->lengthInSeconds();
    }

    /** Method used to build response and log results in case of request error
    * 
    *  @param response: The HTTP response
    *  @param returnCode: The HTTP response return code
    *  @param msg: The HTTP response and log message
    *  @return A boolean indicating Repository if the page was appropriate for request url
    */
    bool respondAndLogError(HttpResponse *response, int returnCode, std::string msg)
    {
        NVJ_LOG->append(NVJ_INFO, msg);
        response->setHttpReturnCode(returnCode);
        return fromString(msg, response);
    }

    /** Method used to decode, build and register individual files.
    * 
    *  @param responseContent: The HTTP response content
    *  @param response: The HTTP response
    *  @param encodedContent: The HTTP response and log message
    *  @param submittedBy: the uploader's token
    */
    void treatFile(std::string& responseContent, HttpResponse *response, std::string encodedContent, int submittedBy)
    {
        //decode encoded data
        std::string decodedString = decoder.decode(encodedContent);

        //insert in file
        std::string filePath = std::string(UPLOAD_DIR) + BASE_FILE_NAME + std::to_string(songNo) + MP3_FILE_EXT;
        std::ofstream file(filePath);
        if (!file)
        {
            responseContent += UPLOAD_PROBLEM_MSG;
            NVJ_LOG->append(NVJ_INFO, "There was an error opening file (path = " + filePath + ")during upload.");
            response->setHttpReturnCode(SERVER_ERROR_CODE);
        }
        file << decodedString;
        file.close();

        //verify file is mp3 and generate json file for mp3 headers
        std::string submitter = ListContainer::getInstance()->getUserList()->getUserName(submittedBy);
        int mp3HeaderGenerationCode = generateMp3HeaderFile(filePath, submitter);
        if (mp3HeaderGenerationCode != NO_PROBLEM_CODE)
        {
            //notify of non mp3 upload attempt and remove file
            responseContent += INVALID_MP3_MSG;
            response->setHttpReturnCode(mp3HeaderGenerationCode);
            if (remove(filePath.c_str()) != 0)
                NVJ_LOG->append(NVJ_INFO, "There was an error removing non mp3 file " + filePath + ".");
            return;
        }
        std::string title = getTitle(filePath);
            
        //notify of mp3 file upload success and add song to queue. 
        NVJ_LOG->append(NVJ_INFO, "User " + submitter + " has successfully uploaded mp3 file " + title + ".");
        responseContent += UPLOAD_SUCCESS_MSG;
        response->setHttpReturnCode(OK_CODE);

        
        ListContainer::getInstance()->getSongList()->addSongToQueue(filePath, getSongLength(filePath));
        ListContainer::getInstance()->getUserList()->addSong(filePath, submittedBy);
    }

    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating AdminDynamicRepository if the page was appropriate for request url
    */
    bool getPage(HttpRequest *request, HttpResponse *response)
    {
        int submittedBy = findSubmitter(request->getUrl());
        std::string submitterName = ListContainer::getInstance()->getUserList()->getUserName(submittedBy);

        if (checkIfBanned(request->getUrl()))
        {
            std::string logErrMsg = "Client " + submitterName + " attempted to upload mp3 when banned.";
            NVJ_LOG->append(NVJ_ERROR, logErrMsg);
            response->setHttpReturnCode(ACCESS_REFUSED_CODE);
            return fromString("You are blocked!", response);
        }
 
        //check several restrictions for upload validity
        if (request->getRequestType() != POST_METHOD)
            return respondAndLogError(response, WRONG_METHOD_CODE, 
                    "Client " + submitterName + " attempted to upload MP3 file with non POST REST call.");

        if (ListContainer::getInstance()->getUserList()->getNumberOfSongsSubmitted(submittedBy) >= MAX_SONGS_USER)
            return respondAndLogError(response, SONG_LIMIT_CODE, "Client " + submitterName + " has reached limit of 5 songs submitted.");
        
        if (ListContainer::getInstance()->getSongList()->getSongQueue().size() >= MAX_SONGS)
            return respondAndLogError(response, SONG_LIMIT_CODE, "The queue has reached it's limit of 10 songs.");
        
        //start checking and uploading files
        std::string responseContent = "";

        std::vector<uint8_t> content = request->getPayload();
        if (content.empty())
        {
            std::string errMsg = "Client " + submitterName + " attempted to upload with wrong request content.";
            return respondAndLogError(response, BAD_CONTENT_CODE, errMsg);
        }

        std::string songBase64 (content.begin(), content.end());
        // std::cout << "song base 64" << songBase64 << std::endl;
/*
        json requestContent;      
        try
        {
            requestContent = json::parse(content.begin(), content.end());
        }
        catch(nlohmann::detail::parse_error e)
        {
            std::string errMsg = "Client " + submitterName + " attempted to upload with faulty request content.";
            return respondAndLogError(response, BAD_CONTENT_CODE, errMsg);
        }
*/

        //for every json section, treat file (decode, check if mp3, generate json header, insert in queue)
        // for (json::iterator it = requestContent.begin(); it != requestContent.end(); ++it)
        // {
            std::cout << "Length " << songBase64.size() << std::endl;
            treatFile(responseContent, response, songBase64, submittedBy);
            if (ListContainer::getInstance()->getUserList()->getNumberOfSongsSubmitted(submittedBy) > MAX_SONGS_USER)
            {
                responseContent = MAX_SONG_MSG;
 //               break;
            }
                
        //}

        return fromString(responseContent, response);
    }
};