/*
SupervisorList :    Data structure class that encapsulates supervisors.
                    This list stores and accesses existing supervisor accounts.
                    This list is contained by the Singleton ListContainer class.
*/
#ifndef SUP_LIST
#define SUP_LIST

#include <map>
#include <list>
#include <algorithm>
#include <signal.h>
#include <semaphore.h>
#include <memory>
#include <vector>

#include "libnavajo/LogStdOutput.hh"
#include "libnavajo/LogFile.hh"

#include "Supervisor.h"

#define SUPERVISORS_FILE_PATH "./supervisors.txt"
#define TEMP_FILE_PATH "./temp.txt"
#define SUPERVISORS_FILE_ERROR "There was an error opening supervisors file."

class SupervisorList
{
public:
    SupervisorList() 
    {
        initializeMutex();
        loadDBInfo();
    }
    
    /** Method used to close device at server shutdown.
    */
    void close()
    {
        sem_close(&supMutex);
    }

    /** Method used by CreateSupervisorPage (admin services) to create new supervisor account.
    * 
    *  @param name: The new supervisor's name
    *  @param password: The new supervisor's password
    */
    void addSupervisor(std::string name, std::string password)
    {
        lockMutex();
        supervisors.insert(std::pair<std::string, std::shared_ptr<Supervisor>>(name, std::make_shared<Supervisor>(name, password)));
        addToDB(name, password);
        unlockMutex();
    }

    /** Method used by DeleteSupervisorPage (admin services) to delete supervisor account.
    * 
    *  @param name: The supervisor's name
    */
    bool removeSupervisor(std::string name)
    {
        std::map<std::string, std::shared_ptr<Supervisor>>::iterator it;
        bool found = false;

        lockMutex();
        it = supervisors.find(name);
        if (it != supervisors.end())
        {
            found = true;
            supervisors.erase(it);
            removeFromDB(name);
        }
        unlockMutex();

        return found;
    }

    /** Method used by CreateSupervisorPage (admin services) to check if a supervisor with this name already exists.
    * 
    *  @param name: The supervisor's name
    */
    bool supervisorExists(std::string name)
    {
        if (supervisors.find(name) != supervisors.end())
            return true;
        return false;
    }

    /** Method used by various supervisor services to check if logged in
    * 
    *  @param name: The supervisor's name
    */
    bool isSupervisorLoggedOn(std::string name)
    {
        bool logged = false;

        lockMutex();
        std::map<std::string, std::shared_ptr<Supervisor>>::iterator it = supervisors.find(name);
        if (it != supervisors.end())
            logged = it->second->isLoggedOn();
        unlockMutex();
            
        return logged;
    }

    /** Method used by SupLoginPage to compare password with request entry on login
    * 
    *  @param name: The supervisor's name
    */
    std::string getSupervisorPassword(std::string name)
    {
        lockMutex();
        std::map<std::string, std::shared_ptr<Supervisor>>::iterator it = supervisors.find(name);
        unlockMutex();
        if (it != supervisors.end())
            return it->second->getpassword();
        return NULL;
    }

    /** Method used by SupChangePwdPage to change supervisor password
    * 
    *  @param name: The supervisor's name
    *  @param newPass: The supervisor's new password
    */
    void setSupervisorPassword(std::string name, std::string newPass)
    {
        lockMutex();
        std::map<std::string, std::shared_ptr<Supervisor>>::iterator it = supervisors.find(name);
        if (it != supervisors.end())
        {
            it->second->setPassword(newPass);
            changeInDB(name, newPass);
        }          
        unlockMutex();
    }

    /** Method used by SupLoginPage to login a supervisor
    * 
    *  @param name: The supervisor's name
    */
    void loginSupervisor(std::string name)
    {
        lockMutex();
        std::map<std::string, std::shared_ptr<Supervisor>>::iterator it = supervisors.find(name);
        if (it != supervisors.end())
            it->second->login();
        unlockMutex();
    }

    /** Method used by SupLogoutPage to logout a supervisor
    * 
    *  @param name: The supervisor's name
    */
    void logoutSupervisor(std::string name)
    {
        lockMutex();
        std::map<std::string, std::shared_ptr<Supervisor>>::iterator it = supervisors.find(name);
        if (it != supervisors.end())
            it->second->logout();
        unlockMutex();

    }

    /** Method used by SupervisorDynamicRepository to associate services url with 
     *  existing supervisor at server startup.
    * 
    *  @return The supervisor names
    */
    std::vector<std::string> getSupervisorNames()
    {
        std::vector<std::string> names;
        std::map<std::string, std::shared_ptr<Supervisor>>::iterator it;

        lockMutex();
        for (it = supervisors.begin(); it != supervisors.end(); ++it)
        {
            names.push_back(it->first);
        }
        unlockMutex();

        return names;
    }

private:
    std::map<std::string, std::shared_ptr<Supervisor>> supervisors; 
    sem_t supMutex;

    void initializeMutex()
    {
        if (sem_init(&supMutex, 0, 1) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to initialize mutex for supervisor list.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void lockMutex()
    {
        if (sem_wait(&supMutex) != 0)
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to wait mutex for supervisor list.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    void unlockMutex()
    {
        if (sem_post(&supMutex))
        {
            NVJ_LOG->append(NVJ_ERROR, "Failed to post mutex for supervisor list.");
            if (raise(SIGINT) != 0) //call exit function through SIGINT signal
                exit(1);
        }
    }

    /*
    Supervisor accounts DB interraction methods
    */
    void addToDB(std::string name, std::string password)
    {
        std::ofstream supFile(SUPERVISORS_FILE_PATH,  std::ios_base::app | std::ios_base::out);
        if (!supFile)
        {
            NVJ_LOG->append(NVJ_INFO, SUPERVISORS_FILE_ERROR);
            return;
        }
        std::string newLine(name + " " + password);
        supFile << newLine;
        supFile.close();
    }

    void removeFromDB(std::string name)
    {
        std::ifstream supFile(SUPERVISORS_FILE_PATH);
        std::ofstream tempFile(TEMP_FILE_PATH);
        if (!supFile || !tempFile)
        {
            NVJ_LOG->append(NVJ_INFO, SUPERVISORS_FILE_ERROR);
            return;
        }
        for( std::string line; std::getline( supFile, line ); )
        {
            size_t firstOf = line.find_first_of(" ");

            if (line.substr(0, firstOf) != name)
                tempFile << line;
        }
        supFile.close();
        tempFile.close();

        remove(SUPERVISORS_FILE_PATH);
        rename(TEMP_FILE_PATH, SUPERVISORS_FILE_PATH);
    }

    void changeInDB(std::string name, std::string newPass)
    {
        std::ifstream supFile(SUPERVISORS_FILE_PATH);
        std::ofstream tempFile(TEMP_FILE_PATH);
        if (!supFile || !tempFile)
        {
            NVJ_LOG->append(NVJ_INFO, SUPERVISORS_FILE_ERROR);
            return;
        }
        for( std::string line; std::getline( supFile, line ); )
        {
            size_t firstOf = line.find_first_of(" ");

            if (line.substr(0, firstOf) == name)
                line = name + " " + newPass;
            
            tempFile << line;
        }
        supFile.close();
        tempFile.close();

        remove(SUPERVISORS_FILE_PATH);
        rename(TEMP_FILE_PATH, SUPERVISORS_FILE_PATH);
    }

    void loadDBInfo()
    {
        std::ifstream supFile(SUPERVISORS_FILE_PATH);
        if (!supFile)
        {
            NVJ_LOG->append(NVJ_INFO, SUPERVISORS_FILE_ERROR);
            return;
        }
        for( std::string line; std::getline( supFile, line ); )
        {
            size_t firstOf = line.find_first_of(" ");
            supervisors.insert(std::pair<std::string, std::shared_ptr<Supervisor>>(line.substr(0,firstOf),
                               std::make_shared<Supervisor>(line.substr(0,firstOf), line.substr(firstOf+1))));
        }
        supFile.close();
    }
};

#endif