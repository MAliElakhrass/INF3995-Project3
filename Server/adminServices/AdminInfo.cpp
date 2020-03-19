/*
AdminInfo:  This class encapsulates the administrator information for his username and his password.
            It is contained and accessible through the Singleton ListContainer and is used by 
            the AdminLoginPage.cpp service and the AdminChangePwdPage.cpp service.
*/
#include <string>
#include <fstream>

#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#define PASSWORD_FILE_PATH "password.txt"
#define PASSWORD_FILE_ERROR "There was a problem opening admin password file."

class AdminInfo
{
public:
    AdminInfo()
    {
        userName = "admin";
        obtainPassword();
    }

    std::string getUserName() {return userName; }
    std::string getPassword() { return password; }

    void setPassword(std::string newPass) 
    { 
        password = newPass;
        std::ofstream passFile(PASSWORD_FILE_PATH, std::ios::trunc); 
        if (!passFile)
        {
            NVJ_LOG->append(NVJ_INFO, PASSWORD_FILE_ERROR);
            return;
        }
        passFile << newPass;
    }

private:
    std::string userName;
    std::string password;

    void obtainPassword()
    {
        std::ifstream passFile(PASSWORD_FILE_PATH);
        if (!passFile)
        {
            NVJ_LOG->append(NVJ_INFO, PASSWORD_FILE_ERROR);
            return;
        }
        passFile >> password;
        passFile.close();
    }
};