/*
blacklist : Data structure class that encapsulates blocked users.
            This list stores and accesses users blocked by supervisors.
            This list is contained by the Singleton ListContainer class.
*/
#ifndef BLACK_LIST
#define BLACK_LIST

#include <string>
#include <signal.h>
#include <semaphore.h>
#include <map>
#include <memory>

#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "BlockedUser.h"

class Blacklist
{
public:

    Blacklist() 
    {
        initializeMutex();
    }
    
    /** Method used to close device at server shutdown.
    */
    void close()
    {
        sem_close(&blackListMutex);
    }

    /** Method used by SupBlockUserPage to block user
    * 
    *  @param token: The user's token
    *  @param userMAC: The user's MAC addr
    *  @param userIP: The user's IP addr
    *  @param userName: The user's name
    */
    void blockUser(int token, std::string userMAC, std::string userIP, std::string userName)
    {
        lockMutex();
        blockedUsers.insert(std::pair<int, std::shared_ptr<BlockedUser>>(token, std::make_shared<BlockedUser>(userName, userIP, userMAC)));
        unlockMutex();
    }

    /** Method used by SupUnblockUserPage to unblock user
    * 
    *  @param userMAC: The user's MAC addr
    *  @return A boolean indicating if a user with this MAC addr was blocked
    */
    bool unblockUser(std::string userMAC)
    {
        bool found = false;
        lockMutex();
        for (std::map<int, std::shared_ptr<BlockedUser>>::iterator it = blockedUsers.begin(); it != blockedUsers.end(); ++it)
        {
            if (it->second->getMAC() == userMAC)
            {
                blockedUsers.erase(it);
                found = true;
                break;
            }
        }   
        unlockMutex();
        return found;
    }

    /** Method used by various user services to check if requester is banned
    * 
    *  @param token: The requester's token
    */
    bool isUserBlocked(int token)
    {
        lockMutex();
        bool found = blockedUsers.find(token) != blockedUsers.end();
        unlockMutex();
        return found;
    }

    /** Method used by SupGetBlacklistPage to obtain a copy of blacklist
    * 
    *  @return A copy of the blacklist
    */
    std::map<int, std::shared_ptr<BlockedUser>> getBlacklist()
    {
        return blockedUsers;
    }

private:
    std::map<int, std::shared_ptr<BlockedUser>> blockedUsers;
    sem_t blackListMutex;

    void initializeMutex()
    {
        if (sem_init(&blackListMutex, 0, 1) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to initialize mutex for black list.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void lockMutex()
    {
        if (sem_wait(&blackListMutex) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to wait mutex for supervisor list.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void unlockMutex()
    {
        if (sem_post(&blackListMutex))
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to post mutex for black list.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }
};

#endif