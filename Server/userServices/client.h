/*
Client : Data structure class that encapsulates information for identified users
         who were attributed a token. 
         This class contains information for a single user and is only used by UserList class.
*/
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <list>

class Client
{
public:
    Client(std::string clientName, std::string clientIP, std::string clientMAC)
    {
        name = clientName;
        ip = clientIP;
        MAC = clientMAC;
    }
    
    std::string getClientName() { return name; }
    std::string getIpAddr() { return ip; }
    std::string getMAC() { return MAC; }
    void addSong(std::string songName) { songsSubmitted.push_back(songName); }
    size_t getNumberOfSongsSubmitted() { return songsSubmitted.size(); }
    
    bool removeSong(std::string songName)
    {
        for (std::string song : songsSubmitted)
        {
            if (song == songName)
            {
                songsSubmitted.remove(song);
                return true;
            }
        }
        return false;
    }

private:
    std::string name;
    std::string ip;
    std::string MAC;
    std::list<std::string> songsSubmitted;
};

#endif