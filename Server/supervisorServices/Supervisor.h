/*
Supervisor :    Data structure class that encapsulates information for supervisor accounts
                This class contains information for a single supervisor and is only used by SupervisorList class.
*/
#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <string>

class Supervisor
{
public:
    Supervisor(std::string supName, std::string supPass)
    {
        name = supName;
        password = supPass;
        loggedOn = false;
    }

    std::string getName() { return name; }
    std::string getpassword() { return password; }
    void setPassword(std::string newPass) { password = newPass; }
    bool isLoggedOn() { return loggedOn; }
    void login() { loggedOn = true; }
    void logout() { loggedOn = false; }

private:
    std::string name;
    std::string password;
    bool loggedOn;
};

#endif