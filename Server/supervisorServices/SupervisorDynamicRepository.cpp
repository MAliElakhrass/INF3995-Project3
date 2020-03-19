/*
SupervisorDynamicRepository:    Repository class responsible for harboring supervisor REST services.
                                Repository classes contain DynamicPage objects that define the services.
                                Repositories receive HTTP requests from server and distribute them to a DynamicPage or
                                pass them on to another Repository.
*/
#ifndef SUP_REPO
#define SUP_REPO

#include <vector>

#include "./pages/SupLoginPage.cpp"
#include "./pages/SupLoginErrorPage.cpp"
#include "./pages/SupLogoutPage.cpp"
#include "./pages/SupChangePwdPage.cpp"
#include "./pages/SupGetSongsPage.cpp"
#include "./pages/SupDeleteSongPage.cpp"
#include "./pages/SupSongPositionPage.cpp"
#include "./pages/SupBlockUserPage.cpp"
#include "./pages/SupUnblockUserPage.cpp"
#include "./pages/SupGetBlacklistPage.cpp"
#include "./pages/SupGetSoundPage.cpp"
#include "./pages/SupVolumeUpPage.cpp"
#include "./pages/SupVolumeDownPage.cpp"
#include "./pages/SupMutePage.cpp"
#include "./pages/SupUnmutePage.cpp"

#define LOGIN_URL_SUP "superviseur/login"
#define SUPERVISOR_BASE_URL "superviseur/"
#define LOGOUT_URL_EXT "/logout"
#define CHANGE_PASS_URL_EXT "/changement_mdp"
#define GET_URL_EXT "/file"
#define DELETE_URL_EXT "/chanson/"
#define POSITION_URL_EXT "/position"
#define BLOCK_USER_URL_EXT "/bloquer"
#define UNBLOCK_USER_URL_EXT "/debloquer"
#define GET_BLACKlIST_URL_EXT "/listenoire"
#define GET_SOUND_URL_EXT "/volume"
#define VOLUME_UP_URL_EXT "/volume/augmenter/"
#define VOLUME_DOWN_URL_EXT "/volume/diminuer/"
#define MUTE_URL_EXT "/volume/sourdine/activer"
#define UNMUTE_URL_EXT "/volume/sourdine/desactiver"

class SupervisorDynamicRepository : public DynamicRepository
{
public:
    SupervisorDynamicRepository() 
    {
        setupServices();
    }

    /** Method used by Admin service CreateSupervisorPage to associate supervisor services with new 
    *   supervisor urls on successful supervisor account creation.
    * 
    *  @param name: the new supervisor's name
    */
    void addURLs(std::string name)
    {
        add(SUPERVISOR_BASE_URL + name + LOGOUT_URL_EXT, &logout);
        add(SUPERVISOR_BASE_URL + name + CHANGE_PASS_URL_EXT, &changePass);
        add(SUPERVISOR_BASE_URL + name + GET_URL_EXT, &getSongs);
        add(SUPERVISOR_BASE_URL + name + POSITION_URL_EXT, &songPosition);
        add(SUPERVISOR_BASE_URL + name + BLOCK_USER_URL_EXT, &blockUser);
        add(SUPERVISOR_BASE_URL + name + UNBLOCK_USER_URL_EXT, &unblockUser);
        add(SUPERVISOR_BASE_URL + name + GET_BLACKlIST_URL_EXT, &getBlacklist);
        add(SUPERVISOR_BASE_URL + name + GET_SOUND_URL_EXT, &volume);
        add(SUPERVISOR_BASE_URL + name + MUTE_URL_EXT, &mute);
        add(SUPERVISOR_BASE_URL + name + UNMUTE_URL_EXT, &unmute);
    }

    /** Method used by Admin service DeleteSupervisorPage to dissociate supervisor services with 
    *   supervisor url on successful supervisor account deletion.
    * 
    *  @param name: the supervisor's name
    */
    void removeURLs(std::string name)
    {
        remove(SUPERVISOR_BASE_URL + name + LOGOUT_URL_EXT);
        remove(SUPERVISOR_BASE_URL + name + CHANGE_PASS_URL_EXT);
        remove(SUPERVISOR_BASE_URL + name + GET_URL_EXT);
        remove(SUPERVISOR_BASE_URL + name + POSITION_URL_EXT);
        remove(SUPERVISOR_BASE_URL + name + BLOCK_USER_URL_EXT);
        remove(SUPERVISOR_BASE_URL + name + UNBLOCK_USER_URL_EXT);
        remove(SUPERVISOR_BASE_URL + name + GET_BLACKlIST_URL_EXT);
    }

private:
    SupLoginPage login;
    SupLoginErrorPage loginError;
    SupLogoutPage logout;
    SupChangePwdPage changePass;
    SupGetSongsPage getSongs;
    SupDeleteSongPage deleteSong;
    SupSongPositionPage songPosition;
    SupBlockUserPage blockUser;
    SupUnblockUserPage unblockUser;
    SupGetBlacklistPage getBlacklist;
    SupGetSoundPage volume;
    SupVolumeUpPage volumeUp;
    SupVolumeDownPage volumeDown;
    SupMutePage mute;
    SupUnmutePage unmute;

    /** Method used by Repository to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating SupervisorDynamicRepository if the page was appropriate for request url
    */
    bool getFile(HttpRequest *request, HttpResponse *response)
	{
        std::string requestUrl = (std::string) request->getUrl();
        
        std::cout << requestUrl << std::endl;

        if (requestUrl == LOGIN_URL_SUP)
        {
            bool success = login.getPage(request, response);
            if (!success)
                return loginError.getPage(request, response);
            else
                return success;
        }

        bool foundPage = DynamicRepository::getFile(request, response);
        if (!foundPage)
            foundPage = checkForSpecialURL(request, response);
        return foundPage;
    }

    /** Method used by constructor to associate request services to existing suervisors
    */
    void setupServices()
    {
        std::vector<std::string> supervisorNames = ListContainer::getInstance()->getSupervisorList()->getSupervisorNames();
        std::vector<std::string>::iterator it;

        for (it = supervisorNames.begin(); it != supervisorNames.end(); ++it)
        {
            addURLs(*it);
        }
    }

    /** Method used by repository if request url matches the delete or volume up or volume down request url structure.
    *   This method is used by repository if no page associated to url was found, as those request url
    *   are more difficult to associate to a page by standard means.
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating if the url matches that of a delete request
    */
    bool checkForSpecialURL(HttpRequest *request, HttpResponse *response)
    {
        //check for superviseur/
        std::string url = request->getUrl();
        std::string tempUrl = SUPERVISOR_BASE_URL;
        size_t firstOf = url.find_first_of(tempUrl);
        if (firstOf != 0 || url.size() <= tempUrl.size())
            return false;
            
        //check for <usager>/
        url = url.substr(tempUrl.size()); 
        firstOf = url.find_first_of("/");
        std::string name = url.substr(0, firstOf);
        if (firstOf == -1 || !ListContainer::getInstance()->getSupervisorList()->supervisorExists(name))
            return false;
        
        //check for /chanson/ or /volume/augmenter/ or /volume/diminuer/
        url = url.substr(firstOf);
        if (checkUrlExtension(url, DELETE_URL_EXT))
        {
            int no;
            tempUrl = DELETE_URL_EXT;
            try
            {
                no = stoi(url.substr(tempUrl.size()));
            }
            catch (std::exception e)
            {
                return false;
            }
            //This is a supervisor delete song url
            deleteSong.setNo(no);
            deleteSong.setName(name);
            return deleteSong.getPage(request, response);
        }
        if (checkUrlExtension(url, VOLUME_UP_URL_EXT))
        {
            int pc;
            tempUrl = VOLUME_UP_URL_EXT;
            try
            {     
                pc = stoi(url.substr(tempUrl.size()));
            }
            catch (std::exception e)
            {
                return false;
            }
            //This is a supervisor volume up url
            volumeUp.setPc(pc);
            volumeUp.setName(name);
            return volumeUp.getPage(request, response);
        }
        if (checkUrlExtension(url, VOLUME_DOWN_URL_EXT))
        {
            int pc;
            tempUrl = VOLUME_DOWN_URL_EXT;
            try
            {
                pc = stoi(url.substr(tempUrl.size()));
            }
            catch (std::exception e)
            {
                return false;
            }
            //This is a supervisor volume down url
            volumeDown.setPc(pc);
            volumeDown.setName(name);
            return volumeDown.getPage(request, response);
        }

        //no match
        return false;
    }

    /** Method used by repository checkForSpecialURL method to check a supervisor request url after <usager>
    * 
    *  @param url: The request url
    *  @param match: The match looked for
    *  @return A boolean indicating if the url contains the match
    */
    bool checkUrlExtension(std::string url, std::string match)
    {
        size_t lastOf = url.find_last_of("/") + 1;
        url = url.substr(0,lastOf);
        if (url == match)
            return true;
        return false;
    }

};

#endif