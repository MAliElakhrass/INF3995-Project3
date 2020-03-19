/*
ListContainer: Singleton class that encapsulates data structures for authenticated users and uploaded songs in queue.
               This class is used by the whole system to gain access to globally used data structures.

Lists contained : 1. userServices/UserList.cpp
                  2. audioPlayer/SongList.cpp
                  3. supervisorServices/SupervisorList.cpp
                  4. supervisorServices/blacklist/Blacklist.cpp
*/
#ifndef LIST_CONTAINER
#define LIST_CONTAINER

#include "userServices/UserList.cpp"
#include "audioPlayer/SongList.cpp"
#include "supervisorServices/SupervisorList.cpp"
#include "supervisorServices/blacklist/Blacklist.cpp"
#include "adminServices/AdminInfo.cpp"

class ListContainer
{
    
public:
    ~ListContainer()
    {
        delete listContainer;
        listContainer = nullptr;
    }

    std::shared_ptr<UserList> getUserList() { return userList; }
    std::shared_ptr<SongList> getSongList() { return songList; }
    std::shared_ptr<SupervisorList> getSupervisorList() { return supList; }
    std::shared_ptr<Blacklist> getBlacklist() { return blacklist; }
    std::shared_ptr<AdminInfo> getAdminInfo() { return adminInfo; }

    void shutdown()
    {
        userList->close();
        songList->close();
        supList->close();
        blacklist->close();
    }
     
    static ListContainer* getInstance(){
        if (!listContainer)
        {
            listContainer = new ListContainer() ; 
        }
        return listContainer;
    }

private:
    std::shared_ptr<UserList> userList;
    std::shared_ptr<SongList> songList;
    std::shared_ptr<SupervisorList> supList;
    std::shared_ptr<Blacklist> blacklist;
    std::shared_ptr<AdminInfo> adminInfo;
    static ListContainer* listContainer; 

    ListContainer() 
    {
        userList = std::make_shared<UserList>();
        songList = std::make_shared<SongList>();
        supList = std::make_shared<SupervisorList>(); 
        blacklist = std::make_shared<Blacklist>();
        adminInfo = std::make_shared<AdminInfo>();
    }
};

ListContainer* ListContainer::listContainer = nullptr;

#endif