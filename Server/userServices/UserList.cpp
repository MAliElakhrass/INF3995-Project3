/*
UserList : Data structure class that encapsulates identified users.
           This list contains and accesses the object instantiations of the Client class.
           This list is contained by the Singleton ListContainer class and is used by 
           most REST service classes to update server state.
*/
#ifndef USER_LIST
#define USER_LIST

#include <map>
#include <list>
#include <memory>
#include <semaphore.h>
#include <signal.h>

#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "client.h"

class UserList
{
public:
    UserList()
    {
        initializeMutex();
    }

    /** Method used to get an identified user's name using attributed token. Used by most services for logging purposes.
    * 
    *  @param token The attributed token
    *  @return The user's name
    */
    std::string getUserName(int token)
    {
        lockMutex();
        std::map<int, std::shared_ptr<Client>>::iterator it = users.find(token);
        std::string name = (it->second)->getClientName();
        unlockMutex();
        return name;
    }

    /** Method used by SupGetSongsPage to obtain additionnal information for supervisor's get song queue service.
    * 
    *  @param token the song uploader's token
    *  @return The uploader's MAC address
    */
    std::string getUserMAC(int token)
    {
        lockMutex();
        std::map<int, std::shared_ptr<Client>>::iterator it = users.find(token);
        std::string addr = (it->second)->getMAC();
        unlockMutex();
        return addr;
    }

    /** Method used by SupGetSongsPage to obtain additionnal information for supervisor's get song queue service.
    * 
    *  @param token the song uploader's token
    *  @return The uploader's IP address
    */
    std::string getUserIp(int token)
    {
        lockMutex();
        std::map<int, std::shared_ptr<Client>>::iterator it = users.find(token);
        std::string addr = (it->second)->getIpAddr();
        unlockMutex();
        return addr;
    }

    /** Method used by userServices/UploaderPage.cpp to get an identified user's number of songs in queue using attributed token
    * 
    *  @param token The attributed token
    *  @return The user's number of songs in queue
    */
    int getNumberOfSongsSubmitted(int token)
    {
        lockMutex();
        std::map<int, std::shared_ptr<Client>>::iterator it = users.find(token);
        int number = (it->second)->getNumberOfSongsSubmitted();
        unlockMutex();
        return number;
    }

    /** Method used by userServices/UploaderPage.cpp to update user's number of songs in queue on file upload
    * 
    *  @param filePath The path of the mp3 file the user uploaded
    *  @param token The attributed token
    */
    void addSong(std::string filePath, int token)
    {
        lockMutex();
        std::map<int, std::shared_ptr<Client>>::iterator it = users.find(token);
        (it->second)->addSong(filePath);
        unlockMutex();
    }

    /** Method used by MediaPlayer thread to update user's number of songs in queue
    * 
    *  @param filePath The path of the mp3 file the user uploaded
    */
    void removeSongFromUser(std::string filePath)
    {
        lockMutex();
        std::map<int, std::shared_ptr<Client>>::iterator it;
        for (it = users.begin(); it != users.end(); ++it)
        {
            if ((it->second)->removeSong(filePath))
                break;
        }
        unlockMutex();
    }

    /** Method used by userServices/IdentificationPage.cpp to add a recently identified user's informations to list
    * 
    *  @param token The attributed token
    *  @param userName The user's name
    *  @param userIP The user's IP address
    *  @param userIP The user's MAC address
    */
    void addUser(int token, std::string userName, std::string userIP, std::string userMAC)
    {
        lockMutex();
        users.insert(std::pair<int, std::shared_ptr<Client>>(token, std::make_shared<Client>(userName, userIP, userMAC)));
        unlockMutex();
    }

    /** Method used by UserDynamicRepository to check if token is valid when checking for delete song url structure
    * 
    *  @param id The token
    *  @return A boolean indicating if token is valid
    */
    bool containsUser(int id)
    {
        bool exists;
        lockMutex();
        exists = (users.find(id) != users.end());
        unlockMutex();
        return exists;
    }

    /** Method used by adminServices/GetStats.cpp to obtain number of users
    * 
    *  @return The number of users
    */
    size_t getNumberOfUsers()
    {
        return users.size();
    }

    /** Method used by many services to obtain a user's token with is name
    * 
    *  @param id The user's name
    *  @return The token
    */
    int getUserToken(std::string userName)
    {
        std::map<int, std::shared_ptr<Client>>::iterator it;
        int token = -1;
        lockMutex();
        for (it = users.begin(); it !=users.end(); ++it)
        {
            if (it->second->getClientName() == userName)
            {
                token = it->first;
                break;
            }           
        }
        unlockMutex();
        return token;
    }

    /** Method used by userServices/IndentificationPage.cpp to verify if a user's information is in the list.
    * 
    *  @param userName The user's name
    *  @param userIP The user's IP address
    *  @param userIP The user's MAC address
    *  @return The user's token if he is already identified or -1 if he isnt.
    **/
    int existUser(std::string userName, std::string userIP, std::string userMAC) {
        
        lockMutex();
        auto it = users.begin();
        int token = -1;
        while(it != users.end()){
            std::shared_ptr<Client> client = it -> second;         

            if (client->getClientName() == userName && client->getIpAddr() == userIP && client->getMAC() == userMAC) {
                token = it->first;
                break;
            }
            it++;
        }
        unlockMutex();
        
        return token;
    }

    /** Method used by supervisorServices/GetBlacklistPage.cpp to obtain the list of all users
    * 
    *  @return The users
    **/
    std::map<int, std::shared_ptr<Client>> getUsers() {
        return users;
    }

    /** Method used by adminServices/GetStats.cpp to obtain number of users
    */
    void close()
    {
        sem_close(&userMutex);
    }

private:
    std::map<int, std::shared_ptr<Client>> users; 
    sem_t userMutex;

    void initializeMutex()
    {
        if (sem_init(&userMutex, 0, 1) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to initialize mutex for user list.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void lockMutex()
    {
        if (sem_wait(&userMutex) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to wait mutex for user queue.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void unlockMutex()
    {
        if (sem_post(&userMutex))
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to post mutex for user queue.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }
};

#endif