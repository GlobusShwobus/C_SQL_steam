#include "steam.h"

namespace ORDO {


    STEAM::STEAM(const std::string& _api_key, const std::string& _user_id) :
        api_key(_api_key), user_id(_user_id) {

        const std::string url = STEAM_URL_CREATOR::CreateSteamURL(URL_TYPE::pl_summary, api_key, user_id);
        LIBMEM recipient;
        LIBCURL_API_CALL::Moshi_moshi_steam_desu_ka(url, recipient);//the most basic version of request

        if (recipient.httpcode == 200) {//if request was not an error then check the containing data

            const nlohmann::json lookfor = nlohmann::json::parse(recipient.memory); //convert char* to json to parse it

            if (lookfor.contains("response") && lookfor["response"].contains("players") && !lookfor["response"]["players"].empty()) {
                LogMessage("Steam User identity established", ISTR_MSG::good);
            }
            else {
                throw std::invalid_argument("Error:: HTTP code 200. Likely API good; user id bad/private");
            }
        }
        else if (recipient.httpcode == 403) {
            throw std::invalid_argument("Error:: HTTP code 403. Likely API key was not confirmed (typo?)");
        }
        else {
            throw std::invalid_argument("Error:: HTTP code " + std::to_string(recipient.httpcode) + " (likely faulty link?)");
        }

    }
    void STEAM::SetupAchievementData(const std::vector<std::pair<std::string, std::string>>& id_list, ACHIEVEMENTS& into)const {

        into.Clear();
        for (const auto& pair : id_list) {
            const std::string app_id = pair.first;
            //need all 3 pieces of data for the SQL table, if any is broken/bad/ugh, none will be added
            std::unique_ptr<nlohmann::json> global_achievements = LIBCURL_API_CALL::Request(ACHIEVEMENT::ach_global, api_key, user_id, app_id);
            std::unique_ptr<nlohmann::json> player_achievements = LIBCURL_API_CALL::Request(ACHIEVEMENT::ach_player, api_key, user_id, app_id);
            std::unique_ptr<nlohmann::json> schema_achievements = LIBCURL_API_CALL::Request(ACHIEVEMENT::ach_schema, api_key, user_id, app_id);

            if (into.Add(std::move(global_achievements), std::move(player_achievements), std::move(schema_achievements))) {
                PrintM(std::string("\nData recieved for >>> ") + pair.second, ISTR_MSG::good);
            }
            else {
                PrintM(std::string("\nData Insertion skiped due to empty data >>> ") + pair.second, ISTR_MSG::fail);
            }
        }
    }

    void STEAM::RequestPlayerRelated(DATA& data)const {
        try {
            auto type = static_cast<PLAYER>(data.type);//DATA stores the general version of ENTITY which is dangerous, thus the cast to PLAYER version to satisfy the requirement of Request

            if (data.Add(std::move(LIBCURL_API_CALL::Request(type, api_key, user_id)))) {//attempts to add the json, if it fails unique_ptr will deal with it
                LogMessage(std::string("Data recieved for >>> ") + TypeToStr(data.type), ISTR_MSG::good);
            }
            else {
                LogMessage(std::string("\nData Insertion skiped due to empty data >>> ") + TypeToStr(data.type), ISTR_MSG::fail);
            }
        }
        catch (const std::exception& e) {
            LogError(std::string("RequestPlayerRelated Error: ") + e.what(), ISTR_ERROR::hard);
        }
    }
    void STEAM::RequestAchievements(ACHIEVEMENTS& data)const {
        try {
            auto owned_games_list = GetOwnedAppIDs();

            SetupAchievementData(*owned_games_list, data);
            HSTR::Wait();
        }
        catch (std::exception& e) {
            LogError(e.what(), ISTR_ERROR::hard);
        }
    }
    void STEAM::ReqeustAchievementsByChoice(ACHIEVEMENTS& data)const {
        try {

            auto owned_games_list = GetOwnedAppIDs();

            auto wishlist = std::make_unique<std::vector<std::pair<std::string, std::string>>>();

            std::string err_msg = "";

            while (true) {

                PrintList(*owned_games_list);
                PrintM("\n\nCurrent Wishlsit::\n=====================================================================================\n", ISTR_MSG::other);
                PrintList(*wishlist);
                PrintM(err_msg, ISTR_MSG::fail);
                printf("\n\nInput >>> ");


                const int input_index = Select(owned_games_list->size());

                if (input_index == 0) {//input functions are pepega currently. when enter (return key) is pressed it has to return a value if no value was entered, thus 0
                    break;
                }
                const int zero_indexed = input_index - 1;//on screen data is showcased from 1, but containers are 0 indexed, so the -1 fix
                if (AssignSelected(zero_indexed, *owned_games_list, *wishlist)) {
                    err_msg = "";
                }
                else {
                    err_msg = "\n" + (*owned_games_list)[zero_indexed].second + ":: already selected or invalid index";
                }
            }
            SetupAchievementData(*wishlist, data);
            HSTR::Wait();
        }
        catch (const std::exception& e) {
            LogError(std::string("RequestSelectionList Error:: ") + e.what(), ISTR_ERROR::fail);
        }
    }

    std::unique_ptr<std::vector<std::pair<std::string, std::string>>> STEAM::GetOwnedAppIDs()const {
        auto list = LIBCURL_API_CALL::Request(PLAYER::pl_games, api_key, user_id);

        if (list == nullptr) {
            throw std::runtime_error("STEAM::GetOwnedAppIDs Recieved nullptr");
        }
        if (list->empty()) {
            throw std::runtime_error("STEAM::GetOwnedAppIDs: Received empty batch of data.");
        }

        if (!(list->contains("response") && (*list)["response"].is_object() && (*list)["response"].contains("games"))) {
            throw std::runtime_error("STEAM::GetOwnedAppIDs: recieved JSON object does not contain required structure");
        }

        std::unique_ptr<std::vector<std::pair<std::string, std::string>>> ids = std::make_unique<std::vector<std::pair<std::string, std::string>>>();

        for (const auto& entry : (*list)["response"]["games"]) {
            const int id = entry["appid"];
            ids->push_back({ std::to_string(id), entry["name"] });
        }

        return ids;
    }
    bool STEAM::AssignSelected(int input, const std::vector<std::pair<std::string, std::string>>& from, std::vector<std::pair<std::string, std::string>>& wishlist)const {

        if (input >= 0 && input < from.size()) {
            if (std::find(wishlist.begin(), wishlist.end(), from[input]) == wishlist.end()) {
                wishlist.push_back(from[input]);
                return true;
            }
        }
        return false;
    }
    const std::string STEAM::TypeToStr(const ENTITY type)const {
        switch (type) {
        case ENTITY::pl_summary:  return "*summary*";
        case ENTITY::pl_recent:   return "*recent activity*";
        case ENTITY::pl_games:    return "*games list*";
        case ENTITY::ach_global:  return "*global achievement stat*";
        case ENTITY::ach_player:  return "player achievement stat*";
        case ENTITY::ach_schema:  return "schema achievement stat*";
        }
        throw std::invalid_argument("STEAM::TypeToStr error type");
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    void STEAM::STEAM_URL_CREATOR::InsertSteamAPI(std::string& template_url, const std::string& api_key) {
        size_t key = template_url.find("key");

        if (key != std::string::npos) {
            size_t delimiter = template_url.find_first_of("=", key) + 1;

            template_url.insert(delimiter, api_key);
        }
    }
    void STEAM::STEAM_URL_CREATOR::InsertSteamUserID(std::string& template_url, const std::string& user_id) {
        size_t key = template_url.find("steamid");

        if (key != std::string::npos) {
            size_t delimiter = template_url.find_first_of("=", key) + 1;

            template_url.insert(delimiter, user_id);
        }
    }
    void STEAM::STEAM_URL_CREATOR::InsertSteamAppID(std::string& template_url, const std::string& app_id) {
        size_t key = template_url.find("appid");

        if (key == std::string::npos) {
            key = template_url.find("gameid");
        }

        if (key != std::string::npos) {
            size_t delimiter = template_url.find_first_of("=", key) + 1;

            template_url.insert(delimiter, app_id);
        }
    }

    std::string STEAM::STEAM_URL_CREATOR::SteamTemplateURLs(const URL_TYPE type) {
        switch (type) {
        case URL_TYPE::ach_schema:      return "https://api.steampowered.com/ISteamUserStats/GetSchemaForGame/v0001/?appid=&name=1&description=1&key=&steamid=&format=json";
        case URL_TYPE::ach_global:      return "https://api.steampowered.com/ISteamUserStats/GetGlobalAchievementPercentagesForApp/v0002/?gameid=&format=json";
        case URL_TYPE::ach_player:      return "https://api.steampowered.com/ISteamUserStats/GetPlayerAchievements/v0001/?appid=&key=&steamid=&name=1&format=json";
        case URL_TYPE::pl_summary:      return "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=&steamids=&format=json";
        case URL_TYPE::pl_games:        return "https://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=&steamid=&include_appinfo=true&include_played_free_games=true&format=json";
        case URL_TYPE::pl_recent:       return "https://api.steampowered.com/IPlayerService/GetRecentlyPlayedGames/v0001/?key=&steamid=&format=json";
        default:
            throw std::invalid_argument("STEAM_URL_CREATOR::SteamTemplateURLs unknown URL_TYPE");
        }
    }
    std::string STEAM::STEAM_URL_CREATOR::CreateSteamURL(const URL_TYPE type, const std::string& api_key, const std::string& user_id) {

        std::string url_template = SteamTemplateURLs(type);

        InsertSteamAPI(url_template, api_key);
        InsertSteamUserID(url_template, user_id);

        return url_template;
    }
    std::string STEAM::STEAM_URL_CREATOR::CreateSteamURL(const URL_TYPE type, const std::string& api_key, const std::string& user_id, const std::string& app_id) {
        std::string url_template = SteamTemplateURLs(type);

        InsertSteamAPI(url_template, api_key);
        InsertSteamUserID(url_template, user_id);
        InsertSteamAppID(url_template, app_id);

        return url_template;
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    size_t STEAM::LIBCURL_API_CALL::MemCallBack(void* contents, size_t size, size_t new_mem_blocks, void* user_response) {

        size_t real_size = size * new_mem_blocks;
        LIBMEM* mem = (LIBMEM*)user_response;

        char* ptr = (char*)realloc(mem->memory, mem->size + real_size + 1);

        if (!ptr) {
            LogMessage("LIBCURL::MemCallBack: Not enough memory (realloc returned NULL)", ISTR_MSG::other);
            return 0;
        }

        mem->memory = ptr;
        memcpy(&(mem->memory[mem->size]), contents, real_size);
        mem->size += real_size;
        mem->memory[mem->size] = 0;

        return real_size;
    }

    void STEAM::LIBCURL_API_CALL::Moshi_moshi_steam_desu_ka(const std::string& url, LIBMEM& data) {

        CURL* curl;
        CURLcode result;

        curl = curl_easy_init();

        if (curl == NULL) {
            throw std::runtime_error("Libcurl curl_easy_init() failure");
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//fuck my ass, insert url as string -> get wider asshole

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, LIBCURL_API_CALL::MemCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

        curl_easy_setopt(curl, CURLOPT_CAINFO, CURL_CERT_PATH);

        result = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &data.httpcode);
        if (result != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw std::runtime_error(std::string("Libcurl err:: ") + curl_easy_strerror(result));
        }

        curl_easy_cleanup(curl);
    }

    std::unique_ptr<nlohmann::json> STEAM::LIBCURL_API_CALL::Request(const PLAYER type, const std::string& api_key, const std::string& user_id) {

        const std::string url = STEAM_URL_CREATOR::CreateSteamURL(static_cast<URL_TYPE>(type), api_key, user_id);
        LIBMEM mem_store;

        Moshi_moshi_steam_desu_ka(url, mem_store);

        return std::make_unique<nlohmann::json>(nlohmann::json::parse(mem_store.memory));
    }

    std::unique_ptr<nlohmann::json> STEAM::LIBCURL_API_CALL::Request(const ACHIEVEMENT type, const std::string& api_key, const std::string& user_id, const std::string& app_id) {

        const std::string url = STEAM_URL_CREATOR::CreateSteamURL(static_cast<URL_TYPE>(type), api_key, user_id, app_id);
        LIBMEM mem_store;

        Moshi_moshi_steam_desu_ka(url.c_str(), mem_store);

        return std::make_unique<nlohmann::json>(nlohmann::json::parse(mem_store.memory));
    }

}