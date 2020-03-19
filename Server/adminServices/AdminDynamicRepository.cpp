/*
AdminDynamicRepository: Repository class responsible for harboring administrator REST services.
                        Repository classes contain DynamicPage objects that define the services.
                        Repositories receive HTTP requests from server and distribute them to a DynamicPage or
                        pass them on to another Repository.
*/
#include "pages/LoginPage.cpp"
#include "pages/LoginErrorPage.cpp"
#include "pages/LogoutPage.cpp"
#include "pages/LogoutErrorPage.cpp"
#include "pages/ChangeAdminPwdPage.cpp"
#include "pages/GetLogsPage.cpp"
#include "pages/UnauthorizedPage.cpp"
#include "pages/CreateSupervisorPage.cpp"
#include "pages/DeleteSupervisorPage.cpp"
#include "pages/GetStatsPage.cpp"
#include "../sharedPages/SyntaxErrorPage.cpp"

#define LOGOUT_URL "admin/logout"
#define LOGIN_URL "admin/login"
#define CHANGE_PASS_URL_ADMIN "admin/changement_mdp"
#define GET_LOGS_URL "admin/logs"
#define CREATE_SUP_URL "admin/creationcompte"
#define DELETE_SUP_URL "admin/suppressioncompte"
#define GET_STATS_URL "admin/statistiques"

class AdminDynamicRepository : public DynamicRepository
{
public:
    AdminDynamicRepository(std::shared_ptr<SupervisorDynamicRepository> repo) 
    {
        createSup.setRepo(repo);
        deleteSup.setRepo(repo);
        loggedOn = false;
        add(GET_LOGS_URL, &logs);
        add(CREATE_SUP_URL, &createSup);
        add(DELETE_SUP_URL, &deleteSup);
        add(GET_STATS_URL, &stats);
        add(CHANGE_PASS_URL_ADMIN, &changePass);
    }

private:
    bool loggedOn;

    LoginPage login;
    LoginErrorPage loginError;
    LogoutPage logout;
    LogoutErrorPage logoutError;
    ChangeAdminPwdPage changePass;
    GetLogsPage logs;
    UnauthorizedPage unauthorized;
    SyntaxErrorPage syntaxError;
    CreateSupervisorPage createSup;
    DeleteSupervisorPage deleteSup;
    GetStatsPage stats;

    /** Method used by server to react to HTTP request
    * 
    *  @param request The HTTP request
    *  @param response The HTTP response
    *  @return A boolean indicating if the service was found by this repository
    */
	bool getFile(HttpRequest *request, HttpResponse *response)
	{
        std::string requestUrl = (std::string) request->getUrl();
        
        std::cout << requestUrl << std::endl;

        //check for login service 
        if (requestUrl == LOGIN_URL)
        {
            bool success = login.getPage(request, response);
            if (success)
            {
                loggedOn = true;
                return success;
            }
            else
            {
                return loginError.getPage(request, response);
            }
        }

        //check for logout service
        if (requestUrl == LOGOUT_URL)
        {
            if (!loggedOn || request->getRequestType() != POST_METHOD)
            {
                return logoutError.getPage(request, response);
            }
            else
            {
                loggedOn = false;
                return logout.getPage(request, response);
            }
        }

        //check if url is valid but admin is logged off
        if (checkIfURLexists(requestUrl) && !loggedOn)
            return unauthorized.getPage(request, response);
        
        //check if service exists
		bool pageFound = DynamicRepository::getFile(request, response);
		if (!pageFound)
			return syntaxError.getPage(request, response);
		return pageFound;
	}

    //this method is used to check if request url is valid for admin services
    bool checkIfURLexists(std::string url)
    {
        return (url == GET_LOGS_URL || url == CREATE_SUP_URL ||
                url == DELETE_SUP_URL || url == GET_STATS_URL ||
                url == CHANGE_PASS_URL_ADMIN);
    }
};