/*
BlockedUser :   Data structure class that encapsulates information for blocked users
                This class contains information for a single user and is only used by Blacklist class.
*/
#ifndef BLOCKED_USER  
#define BLOCKED_USER

#include <string>

class BlockedUser
{
public:
    BlockedUser(std::string userName, std::string userIP, std::string userMAC)
    {
        name = userName;
        ip = userIP;
        MAC = userMAC;
    }

    std::string getName() { return name; }
    std::string getIp() { return ip; }
    std::string getMAC() { return MAC; }

private:
    std::string name;
    std::string ip;
    std::string MAC;
};

#endif