/*
Constants that are used by many classes are stored here
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

//FILE CONSTANTS
#define UPLOAD_DIR "./files/mp3Files/"
#define JSON_DIR "./files/jsonFiles/"
#define JSON_FILE_INDENTATION 4
#define JSON_FILE_EXT ".json"
#define MP3_FILE_EXT ".mp3"
#define MP3_FILE_PROBLEM_MSG "there was an error opening mp3 file."
#define JSON_FILE_PROBLEM_MSG "there was an error opening json header file."
#define LOG_FILE_PATH "./logs/serverLogs.log"


//HTTP response codes
#define OK_CODE 200
#define BAD_CONTENT_CODE 400
#define UNAUTHENTIFIED_CODE 401
#define ACCESS_REFUSED_CODE 403
#define WRONG_METHOD_CODE 405
#define SONG_LIMIT_CODE 413
#define INVALID_MP3_CODE 415
#define SERVER_ERROR_CODE 500


//Wrong method response messages
#define USE_POST_MSG "You must use a POST method for this service"
#define USE_DELETE_MSG "You must use a DELETE method for this service"
#define USE_GET_MSG "You must use a GET method for this service"


//supervisor and user shared delete requests constants
#define INEXISTANT_SONG "Non existant song number"
#define SONG_FILE_PROBLEM "There was a problem opening song's json header file"
#define DELETE_SUCCESS_MSG "You have successfully removed song."

//General response content messages
#define WRONG_CONTENT_MSG "Unknown user attempted to identify with wrong request content."
#define LOGGED_OUT_MSG "You must be logged in for this."
#define LOGIN_SUCCESS_MSG "You have successfuly logged in."
#define LOGOUT_SUCCESS_MSG "You have successfuly logged off."

//general user error messages
#define BANNED_ERROR_MSG "You have been blocked!"


//general supervisor error messages
#define WRONG_LOGIN_INFO "The username or password entered is incorrect."
#define WRONG_CONTENT_SUP_MSG "There was a problem with your request content."
#define ID_INFO_PROBLEM_MSG "There is a problem with the information you have entered."

//general administrator error messages
#define ENCRYPTION_ERROR "error"

//song list shared constant
#define NONE "none"


#endif