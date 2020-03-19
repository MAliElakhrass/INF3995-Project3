/*
UserDynamicRepository: Repository class responsible for harboring regular user REST services.
                        Repository classes contain DynamicPage objects that define the services.
                        Repositories receive HTTP requests from server and distribute them to a DynamicPage or
                        pass them on to another Repository.
*/
#include "../sharedPages/SyntaxErrorPage.cpp"
#include "pages/IdentificationErrorPage.cpp"
#include "pages/UploaderPage.cpp"
#include "pages/SongListPage.cpp"
#include "pages/IdentificationPage.cpp"
#include "pages/DeleteSongPage.cpp"
#include "pages/UserBlockedErrorPage.cpp"

#define IDENTIFICATION_URL "usager/identification"
#define LIST_URL "/usager/file/"
#define UPLOAD_URL "usager/chanson/"
#define DELETE_URL "usager/chanson/"

class UserDynamicRepository : public DynamicRepository
{
    IdentificationErrorPage identificationErrorPage;
    SyntaxErrorPage syntaxErrorPage;
    IdentificationPage identification;
    ListPage listPage;
    MP3Uploader uploader;
    DeleteSongPage deleteSong;
    UserBlockedErrorPage blocked;

    /** Method used by server to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating if the service was found by this repository
    */
	bool getFile(HttpRequest *request, HttpResponse *response)
	{
        std::cout << request->getUrl() << std::endl;
        
        if ((std::string) request->getUrl() == IDENTIFICATION_URL)
        {
            bool idCreated = identification.getPage(request, response);
            if (idCreated)
            {
                //add services for this token
                add(UPLOAD_URL + std::to_string(identification.getLastId()), &uploader);
                add(LIST_URL + std::to_string(identification.getLastId()), &listPage);
                return idCreated;
            }
            else
            {
                return identificationErrorPage.getPage(request, response);
            }
        }

        bool pageFound = DynamicRepository::getFile(request, response);

        if (!pageFound)
            pageFound = checkForDeleteURL(request, response);
        
		if (!pageFound)
			return syntaxErrorPage.getPage(request, response);
		return pageFound;
	}

    /** Method used by repository if request url matches the delete request url structure.
    *   This method is used by repository if no page associated to url was found, as delete request url
    *   is more difficult to associate to a page by standard means.
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating if the url matches that of a delete request
    */
    bool checkForDeleteURL(HttpRequest *request, HttpResponse *response)
    {
        std::string url = request->getUrl();
        std::string tempUrl(DELETE_URL);
        
        //check base request url matches base delete url
        size_t firstOf = url.find_first_not_of(tempUrl);
        if (firstOf != tempUrl.size() || url.size() <= tempUrl.size())
            return false;
        url = url.substr(tempUrl.size() - 1);

        //check rest of request url matchers the <id>/<no> format
        firstOf = url.find_first_of("/");
        size_t lastOf = url.find_last_of("/");
        if (lastOf == -1 || firstOf == -1 || firstOf == lastOf)
            return false;

        //check id validity
        int id = -1;
        int no = -1;
        try
        {
            id = stoi(url.substr(firstOf + 1, lastOf - (firstOf + 1)));
            no = stoi(url.substr(lastOf + 1));
        }
        catch(const std::exception& e)
        {
            return false;
        }  
        if (!ListContainer::getInstance()->getUserList()->containsUser(id))
            return false;     
        
        //check if token's owner is blacklisted
        if (ListContainer::getInstance()->getBlacklist()->isUserBlocked(id))
        {
            blocked.setUserId(id);
            return blocked.getPage(request, response); //this will return true so the server doesnt hand request over to next repository.
        }
        
        //prepare and launch delete request page
        deleteSong.setId(id);
        deleteSong.setNo(no);
        return deleteSong.getPage(request,response); //this will return true so the server doesnt hand request over to next repository.
    }
};