#pragma once

#include "Entity.h"
#include <fstream>
#include <curl/curl.h>


//#define CURL_CERT_PATH "../include/curl_inc/cacert.pem"

/*
This section about making web requests. Importanly libcurl library is used and a certificate is required. If missing,
need manual downlaod from their website. Otherwise the path is defined in a macro.

The main class STEAM is the manager through which operations are conducted. It stores the required API key to access steam data
and the user steam ID. As mentioned in Entity.h It is assumed user data is public access not private/partially private.

STEAM has 3 nested classes declared. One stores the logic to create the correct URLs from predefined URL templates as per steam
web API STEAM_URL_CREATOR. LIBCURL_API_CALL is the wrapper around libcurl. It is basically one to one how it is defined on their own docs in their website.
LIBMEM, is the container that stores data. Again it is as defined on libcurl docs with slight modifications to construction and destruction.

The nested classes should never be used in isolation. While they could work outside of STEAM, they're meaningless without context.
*/


namespace ORDO {

    struct CURL_RETURN;
    class CURL_LINK;

    class STEAM {

        const std::string api_key;
        const std::string user_id;
        
        class STEAM_URL_CREATOR;
        class LIBCURL_API_CALL;
        class LIBMEM;


        //This function is hardwired to make a call to a URL which requests GamesList. The ptr in returns in a temporary used
        //used to extract app keys to then make achievement requests. Otherwise there is no way to know what app keys to use to call
        //achievement data. std::pair stores app key as first, and app name as second
        std::unique_ptr<std::vector<std::pair<std::string, std::string>>> GetOwnedAppIDs()const;


        //Since achievements are three objects disguised as one, this function requires an app keys (aquired through GetOwnedAppIDs)
        //This function makes 3 web requests and attempts to add them in the container.
        void SetupAchievementData(const std::vector<std::pair<std::string, std::string>>& id_list, ACHIEVEMENTS& into)const;


        //Takes input index (after adjusted to zero indexation) and assigns the desired data from 'from' into wishlist if it doesn't already exist
        bool AssignSelected(int input, const std::vector<std::pair<std::string, std::string>>& from, std::vector<std::pair<std::string, std::string>>& wishlist)const;


        //a table function which returns a string representation of the enumeration, used for error/message loging
        const std::string TypeToStr(const ENTITY type)const;
    public:
        STEAM() = delete;

        //This class does not support default init as it's members are const. Additionally the constructor may throw for many reasons
        //The intended use is in a try block. The reason for this is to try to log in until API key and user id are verified.
        //Otherwise there is nothing to do if the basics are not guaranteed.
        //The only case the constructor will not throw is if it recieves http code 200 and the JSON retrieved contains a user (though it does not care what user)
        STEAM(const std::string& _api_key, const std::string& _user_id);


        //This function takes a DATA as parameter (any but the desired ones are through MDATA), makes a web request and moves the JSON to DATA
        void RequestPlayerRelated(DATA& data)const;


        //Requests all of the data for whatever apps the user owns to aquire achievement information
        void RequestAchievements(ACHIEVEMENTS& data)const;


        //This version lets the user pick which games (by name) they wish to extract information for
        void ReqeustAchievementsByChoice(ACHIEVEMENTS& data)const;


        //Used in Database.h for strong relational assignment of table data
        const std::string& CurrentSteamWorkingID()const {
            return user_id;
        }
    private:
        STEAM(const STEAM&) = delete;
        STEAM& operator=(const STEAM&) = delete;
        STEAM(STEAM&& other) noexcept = delete;
    };

    class STEAM::STEAM_URL_CREATOR {
        //The following three functions take a template URL and assign key to a specific location using std::string operations
        static void InsertSteamAPI(std::string& template_url, const std::string& api_key);
        static void InsertSteamUserID(std::string& template_url, const std::string& user_id);
        static void InsertSteamAppID(std::string& template_url, const std::string& app_id);

        //returns a template version of the steam web URLs
        static std::string SteamTemplateURLs(const URL_TYPE type);


    public:


        //used to create a URL for non achievement data
        static std::string CreateSteamURL(const URL_TYPE type, const std::string& api_key, const std::string& user_id);


        //used to create a URL for achievement related data
        static std::string CreateSteamURL(const URL_TYPE type, const std::string& api_key, const std::string& user_id, const std::string& app_id);
    };

    class STEAM::LIBCURL_API_CALL {

        static std::string& CertPath() {
            static bool is_set = false;
            static std::string cert_path;

            if (!is_set) {
                std::ifstream file("cacert.pem");
                if (file.good()) {
                    cert_path = "cacert.pem";
                    is_set = true;
                }
                else {
                    file.open("../include/curl_inc/cacert.pem");
                    if (file.good()) {
                        cert_path = "../include/curl_inc/cacert.pem";
                        is_set = true;
                    }
                    else {
                        throw std::runtime_error("UNABLE TO DETERMINE PATH TO CERTIFICATE");
                    }
                }
            }

            return cert_path;
        }

        //this function is defined as per how libcurl library wants it. used by curl_easy_setopt CURLOPT_WRITEFUNCTION, requires to be super specifc
        static size_t MemCallBack(void* contents, size_t size, size_t new_mem_blocks, void* user_response);

    public:

        //the basic function to set up libcurl web requests, retrives data in char* and httpcode
        //url parameter is string but curl requires strictly a C style string. Possible ambiguous bug if string is inserted.
        //may throw
        static void Moshi_moshi_steam_desu_ka(const std::string& url, LIBMEM& data);


        //The intended way to retrieve data in JSON format, requires api key and user id from STEAM. Creates URL depending on what DATA type STEAM recieved
        static std::unique_ptr<nlohmann::json> Request(const PLAYER type, const std::string& api_key, const std::string& user_id);

        //same as above but for achievements
        static std::unique_ptr<nlohmann::json> Request(const ACHIEVEMENT type, const std::string& api_key, const std::string& user_id, const std::string& app_id);
    };

    //Intended use is for inside functions as strictly temporary with no outside use cases. Automatically initializes memory to at least 1 byte
    //automatically frees memory in destructor
    class STEAM::LIBMEM {
    public:
        char* memory = nullptr;
        long httpcode = 0;
        size_t size = 0;

        LIBMEM() {
            memory = static_cast<char*>(malloc(1));
            if (!memory) {
                throw std::bad_alloc();
            }
            memory[0] = 0;
        }
        ~LIBMEM() {
            if (memory) {
                free(memory);
            }
        }
    private:
        LIBMEM(const LIBMEM&) = delete;
        LIBMEM& operator=(const LIBMEM&) = delete;
        LIBMEM(LIBMEM&& other) noexcept = delete;
    };
}

