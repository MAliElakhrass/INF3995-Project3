/*
Server class : Singleton class that contains libnavajo classes respinsible for REST services.
Main method : Server launch method. Responsible for server launch, 
              audio device setup and song queue data structure/thread launch.
*/
#include <memory>
#include <signal.h>
#include <pthread.h>
#include <experimental/filesystem>

#include "userServices/UserDynamicRepository.cpp"
#include "supervisorServices/SupervisorDynamicRepository.cpp"
#include "adminServices/AdminDynamicRepository.cpp"
#include "audioPlayer/MediaPlayer.cpp"
#include "ListContainer.cpp"
#include "audioPlayer/SoundControl.cpp"

#define HTTP_PORT 80
#define SSL_PORT 443
#define THREAD_SETUP_ERROR_MSG "Failed to start MediaPlayer at server start."
#define DEVICE_SETUP_ERROR_MSG "Failed to setup audio device at server start."
#define SOUND_CONTROL_SETUP_ERROR_MSG "Failed to setup sound control at server start."

#define SERVER_CERT_PATH "../ssl/server/server.crt"
#define CA_PATH "../ssl/CA/rootCA.crt"
#define CLIENT_DN "/C=CA/ST=Quebec/L=Montreal/O=Polytechnique/OU=Montreal/CN=Gabriel/emailAddress=gabriel.cote-jones@polymtl.ca"

class Server
{
public:
    ~Server()
    {
        delete server;
        server = nullptr;
    }

    static Server* getInstance()
    {
        if (server == nullptr)
        {
            server = new Server();
        }
        return server;
    }

    void stopService() 
    {
        deleteFiles();
        secureServices->stopService();
	    userServices->stopService();
    }
   
    void start() 
    { 
        secureServices->startService();
        userServices->startService();
        std::cout << "Server is running." << std::endl;
        userServices->wait(); 
    }

    void deleteFiles()
    {
        for (const auto & entry : std::experimental::filesystem::directory_iterator(UPLOAD_DIR))
        {
            if (access(entry.path().c_str(), R_OK) == 0)
                remove(entry.path().c_str());
        }  
        for (const auto & entry : std::experimental::filesystem::directory_iterator(JSON_DIR))
        {
            if (access(entry.path().c_str(), R_OK) == 0)
                remove(entry.path().c_str());
        }
    }

private:
    static Server* server;

    std::unique_ptr<WebServer> userServices;
    std::unique_ptr<UserDynamicRepository> userRepo;
    std::unique_ptr<WebServer> secureServices;
    std::shared_ptr<SupervisorDynamicRepository> supRepo;
    std::unique_ptr<AdminDynamicRepository> adminRepo;

    Server() 
    {
        userServices = std::make_unique<WebServer>();
	    userServices->setServerPort(HTTP_PORT);
        userRepo = std::make_unique<UserDynamicRepository>();
        userServices->addRepository(userRepo.get());      
        
        secureServices = std::make_unique<WebServer>();
	    secureServices->setServerPort(SSL_PORT);
        supRepo = std::make_shared<SupervisorDynamicRepository>();
        secureServices->addRepository(supRepo.get());
        adminRepo = std::make_unique<AdminDynamicRepository>(supRepo);
        secureServices->addRepository(adminRepo.get());

        //setup secure connexion
        secureServices->setUseSSL(true, SERVER_CERT_PATH);
        secureServices->setAuthPeerSSL(true, CA_PATH);
        secureServices->addAuthPeerDN(CLIENT_DN);
    }

};

Server* Server::server = nullptr;

void exitFunction(int signum)
{
    ListContainer::getInstance()->shutdown();
    SoundControl::getInstance()->closeAlsaVolume();
    Server::getInstance()->stopService();
}

int main()
{
    signal(SIGTERM, exitFunction);
	signal(SIGINT, exitFunction);

    //setup logging output
	NVJ_LOG->addLogOutput(new LogFile(LOG_FILE_PATH));

    //setup audio device
    if (!ListContainer::getInstance()->getSongList()->setupDevice())
    {
        NVJ_LOG->append(NVJ_INFO, DEVICE_SETUP_ERROR_MSG);
        return 0;
    }

    if (!SoundControl::getInstance()->initAlsaVolume())
    {
        NVJ_LOG->append(NVJ_INFO, SOUND_CONTROL_SETUP_ERROR_MSG);
        return 0;
    }

    //start music playing thread
    pthread_t playerThread;
    if (pthread_create(&playerThread, NULL, waitAndPlay, NULL) != 0)
    {
        NVJ_LOG->append(NVJ_INFO, THREAD_SETUP_ERROR_MSG);
        return 0;
    }
        
    //launch server services
    Server::getInstance()->deleteFiles();
	Server::getInstance()->start();

	LogRecorder::freeInstance();

	return 0;
}