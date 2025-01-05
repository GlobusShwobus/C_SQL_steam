#include "Database.h"

namespace ORDO {

    void DATABASE::InsertPlayer(const DATA& data) {
        if (!isWorkingSchema()) {
            LogMessage("A working schema has not been set", ISTR_MSG::fail);
            return;
        }
        if (data.Read().empty()) {
            LogMessage("Continer is empty... Nothing to execute", ISTR_MSG::fail);
            return;
        }
        try {
            auto conn = Connect();

            CreateTable(*conn, data.type);//may do nothing if exists but can throw

            const std::string insert_statement = SETUP_STATEMENTS::InsertTableStmt(data.type);

            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(insert_statement));

            IO_JSON_SQL::InsertData(data.Read(), steam_id, data.type, *pstmt);
        }
        catch (const std::exception& e) {
            LogError(std::string("Insertion logic failed: ") + e.what(), ISTR_ERROR::hard);
        }
    }
    void DATABASE::InsertAchievements(const ACHIEVEMENTS& achs) {
        if (!isWorkingSchema()) {
            LogMessage("A working schema has not been set", ISTR_MSG::fail);
            return;
        }

        const std::vector<std::unique_ptr<nlohmann::json>>& global = achs.Read(ACHIEVEMENT::ach_global);
        const std::vector<std::unique_ptr<nlohmann::json>>& player = achs.Read(ACHIEVEMENT::ach_player);
        const std::vector< std::unique_ptr<nlohmann::json>>& schema = achs.Read(ACHIEVEMENT::ach_schema);

        if (global.empty() || player.empty() || schema.empty()) {
            LogMessage("Achievement continers are empty... Nothing to execute", ISTR_MSG::fail);
            return;
        }

        if (!(global.size() == player.size() && player.size() == schema.size())) {
            LogError("DATABASE::InsertAchievements Achievement containers are different sizes. Code error", ISTR_MSG::hard);//should not happen as per how ACHIEVEMENT::Add works, but if it does, it is indeed fucked
            return;
        }


        try {
            auto achievement_list = IO_JSON_SQL::MergeAchievementData(global, player, schema);

            if (achievement_list.empty()) {
                throw std::invalid_argument("DATABASE::InsertAchievements extraction error");
            }

            auto conn = Connect();

            CreateTable(*conn, ENTITY::ach_global);//may do nothing if exists but can throw and is still computation involved
            const std::string insert_statement = SETUP_STATEMENTS::InsertTableStmt(ENTITY::ach_global);

            std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(insert_statement));

            IO_JSON_SQL::InsertData(achievement_list, steam_id, TABLE_TYPE::ach_schema, *pstmt);
        }
        catch (const std::exception& e) {
            LogError(e.what(), ISTR_ERROR::hard);
            return;
        }
    }
    std::unique_ptr<sql::Connection> DATABASE::Connect()const {
        std::unique_ptr<sql::Connection> conn(driver->connect(ip, server_name, server_password));
        conn->setSchema(working_schema);
        return conn;
    }
    bool DATABASE::isWorkingSchema()const {
        return working_schema != "";
    }
    void DATABASE::CreateTable(sql::Connection& conn, const TABLE_TYPE type)const {

        const auto table_list = SETUP_STATEMENTS::GetTablesList(conn, working_schema);

        const std::string table_name = SETUP_STATEMENTS::DefinedNames(type);

        const bool already_exists = StrContains(*table_list, table_name);

        if (!already_exists) {
            std::unique_ptr<sql::Statement> stmt(conn.createStatement());
            stmt->execute(SETUP_STATEMENTS::CreateTableStmt(type));
        }
    }
    void DATABASE::CreateNewSchema()const {
        Refresh();
        printf("\nEnter schema name >>> ");
        const std::string name = InputString(INPUT_T::chars);

        if (name.empty()) {
            LogMessage("Unable to create a no-name schema", ISTR_MSG::fail);
            return;
        }

        const std::string create_stmt = "CREATE DATABASE " + name + ";";

        try {
            std::unique_ptr<sql::Connection> conn(driver->connect(ip, server_name, server_password));
            std::unique_ptr<sql::Statement> stmt(conn->createStatement());

            stmt->execute(create_stmt);

            LogMessage("Schema >> " + name + " << created", ISTR_MSG::good);
        }
        catch (const std::exception& e) {
            LogError(std::string("Failed to create new schema: ") + e.what(), ISTR_ERROR::fail);
        }
    }
    void DATABASE::AssignWorkingSchema() {
        try {
            Refresh();
            printf("\nSet working schema::\n=====================================================================================\n");

            const auto list = GetSchemaList();
            if (list->empty()) {
                LogMessage("There are no schemas in your database", ISTR_MSG::fail);
                return;
            }

            size_t i = 1;
            for (const auto& entry : *list) {
                printf("\n[%zu] %s", i++, entry.c_str());
            }

            printf("\n\nInput >>> ");
            const int select = Select(list->size(), INPUT_T::numsNoZero);

            if (AssignSchemaIndexCheck(select, list->size())) {
                working_schema = (*list)[select - 1];
            }
            else {
                LogMessage("Invalid or out of range input, no schema is set", ISTR_MSG::fail);
            }
        }
        catch (const std::exception& e) {
            LogError(std::string("DATABASE::AssignWorkingSchema failure: ") + e.what(), ISTR_ERROR::fail);
        }

        if (isWorkingSchema()) {
            LogMessage("Working schema set to " + working_schema, ISTR_MSG::good);
        }
    }
    std::unique_ptr<std::vector<std::string>> DATABASE::GetSchemaList()const {
        auto list = std::make_unique<std::vector<std::string>>();

        std::unique_ptr<sql::Connection> conn(driver->connect(ip, server_name, server_password));
        std::unique_ptr<sql::Statement>  stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet>  results(stmt->executeQuery("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME NOT IN ('sys', 'performance_schema', 'information_schema', 'mysql');"));

        while (results->next()) {
            list->push_back(results->getString(1));
        }

        return list;
    }
    bool DATABASE::AssignSchemaIndexCheck(const int select, const size_t size)const {
        const int zero_index_fix = select - 1;
        return zero_index_fix >= 0 && zero_index_fix < size;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<std::vector<std::string>> DATABASE::SETUP_STATEMENTS::GetTablesList(sql::Connection& conn, const std::string& schema_name) {

        auto list = std::make_unique<std::vector<std::string>>();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn.prepareStatement("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.tables WHERE TABLE_SCHEMA = ?"));
        pstmt->setString(1, schema_name);

        std::unique_ptr<sql::ResultSet> results(pstmt->executeQuery());

        while (results->next()) {
            list->push_back(results->getString(1));
        }

        return list;
    }
    const std::string DATABASE::SETUP_STATEMENTS::CreateTableStmt(const TABLE_TYPE type) {
        switch (type) {
        case TABLE_TYPE::pl_summary:     return CSUMMARY_TABLE;
        case TABLE_TYPE::pl_games:       return CGAMES_TABLE;
        case TABLE_TYPE::pl_recent:      return CACTIVITY_TABLE;
        case TABLE_TYPE::ach_global:     //intentional as achievements behave as a single entity
        case TABLE_TYPE::ach_player:     //intentional
        case TABLE_TYPE::ach_schema:     return CACHIEVEMENT_TABLE;
        default:
            throw std::invalid_argument("DATABASE::SETUP_STATEMENTS::CreateTableStmt invalid type recieved");
        }
        return {};
    }
    const std::string DATABASE::SETUP_STATEMENTS::InsertTableStmt(const TABLE_TYPE type) {
        switch (type) {
        case TABLE_TYPE::pl_summary:     return ISUMMARY_TABLE;
        case TABLE_TYPE::pl_games:       return IGAMES_TABLE;
        case TABLE_TYPE::pl_recent:      return IACTIVITY_TABLE;
        case TABLE_TYPE::ach_global:     //intentional as achievements behave as a single entity
        case TABLE_TYPE::ach_player:     //intentional
        case TABLE_TYPE::ach_schema:     return IACHIEVEMENT_TABLE;
        default:
            throw std::invalid_argument("DATABASE::SETUP_STATEMENTS::InsertTableStmt invalid type recieved");
        }
        return {};
    }
    const std::string DATABASE::SETUP_STATEMENTS::DefinedNames(const TABLE_TYPE type) {
        switch (type) {
        case TABLE_TYPE::pl_summary:     return "player_summary";
        case TABLE_TYPE::pl_games:       return "player_owned_games";
        case TABLE_TYPE::pl_recent:      return "activity_log";
        case TABLE_TYPE::ach_global:     //intentional as achievements behave as a single entity
        case TABLE_TYPE::ach_player:     //intentional
        case TABLE_TYPE::ach_schema:     return "achievements";
        default:
            throw std::invalid_argument("DATABASE::SETUP_STATEMENTS::DefinedNames invalid type recieved");
        }
        return {};
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void DATABASE::IO_JSON_SQL::InsertData(const std::vector<std::unique_ptr<nlohmann::json>>& data, const std::string& steam_id, const TABLE_TYPE type, sql::PreparedStatement& pstmt) {
        switch (type) {
        case TABLE_TYPE::pl_summary:     InsertSummary(data, pstmt);               break;
        case TABLE_TYPE::pl_games:       InsertGames(data, pstmt, steam_id);       break;
        case TABLE_TYPE::pl_recent:      InsertActivity(data, pstmt, steam_id);    break;
        case TABLE_TYPE::ach_global:     //intentional as achievements behave as a single entity
        case TABLE_TYPE::ach_player:     //intentional
        case TABLE_TYPE::ach_schema:     InsertAchievements(data, pstmt, steam_id); break;
        default:
            throw std::invalid_argument("DATABASE::IO_JSON_SQL::InsertData invalid type recieved");
        }
    }
    std::vector<std::unique_ptr<nlohmann::json>> DATABASE::IO_JSON_SQL::MergeAchievementData(const std::vector<std::unique_ptr<nlohmann::json>>& global, const std::vector<std::unique_ptr<nlohmann::json>>& player, const std::vector<std::unique_ptr<nlohmann::json>>& schema) {

        std::vector<std::unique_ptr<nlohmann::json>> merged;

        for (size_t index = 0; index < global.size(); ++index) {//for each pointer which stores a json
            nlohmann::json achievement_list = nlohmann::json::array();

            for (const auto& achievement : (*player[index])["playerstats"]["achievements"]) {//for each achievement in the json
                nlohmann::json achievement_entry;
                achievement_entry["gameName"] = (*player[index])["playerstats"]["gameName"];

                try {//if the error occurs here, i want to know, easier to isolate
                    const std::string apiname = achievement["apiname"];
                    ExtractAchivPlayer(achievement, achievement_entry);
                    ExtractAchivGlobal(global, achievement_entry, index, apiname);
                    ExtractAchivSchema(schema, achievement_entry, index, apiname);
                }
                catch (const std::exception& e) {
                    LogError(std::string("DATABASE::IO_JSON_SQL::MergeAchievementData (Likely JSON parsing) ") + e.what(), ISTR_ERROR::fail);
                    merged.clear();
                    return merged;
                }
                achievement_list.push_back(std::move(achievement_entry));
            }
            merged.push_back(std::make_unique<nlohmann::json>(std::move(achievement_list)));
        }

        return merged;
    }
    void DATABASE::IO_JSON_SQL::InsertSummary(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt) {
        for (const std::unique_ptr<nlohmann::json>& json : data) {
            for (const auto& entry : (*json)["response"]["players"]) {

                std::string     player_id = entry["steamid"];
                std::string     persona_name = entry["personaname"];
                long long       last_logoff = entry["lastlogoff"];
                long long       time_created = entry["timecreated"];
                std::string     country_code = entry["loccountrycode"];

                pstmt.setBigInt(1, player_id);
                pstmt.setString(2, persona_name);
                pstmt.setDateTime(3, UnixTime(last_logoff));
                pstmt.setDateTime(4, UnixTime(time_created));
                pstmt.setString(5, country_code);

                pstmt.executeUpdate();

                LogMessage("Insertion success where PRIMARY KEY >> " + player_id + " <<:: User >> " + persona_name + " <<", ISTR_MSG::good);
            }
        }
    }
    void DATABASE::IO_JSON_SQL::InsertGames(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt, const std::string& steam_id) {
        for (const std::unique_ptr<nlohmann::json>& json : data) {
            for (const auto& entry : (*json)["response"]["games"]) {

                const int          game_code = entry["appid"];
                const std::string  id = entry["name"];
                const int          playtime = entry["playtime_forever"];
                const long long    last_played = entry["rtime_last_played"];

                pstmt.setString(1, id);
                pstmt.setBigInt(2, steam_id);
                pstmt.setInt(3, game_code);
                pstmt.setInt(4, playtime);
                pstmt.setDateTime(5, UnixTime(last_played));

                pstmt.executeUpdate();
                LogMessage("Insertion success where PRIMARY KEY >> " + id, ISTR_MSG::good);
            }
        }
    }
    void DATABASE::IO_JSON_SQL::InsertActivity(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt, const std::string& steam_id) {
        std::time_t current_time = std::time(nullptr);
        for (const std::unique_ptr<nlohmann::json>& json : data) {
            for (const auto& entry : (*json)["response"]["games"]) {

                const int          app_id = entry["appid"];
                const std::string  game_name = entry["name"];
                const int          playtime_2weeks = entry["playtime_2weeks"];
                const int          playtime_forever = entry["playtime_forever"];

                pstmt.setInt(1, app_id);
                pstmt.setString(2, game_name);
                pstmt.setInt(3, playtime_2weeks);
                pstmt.setInt(4, playtime_forever);
                pstmt.setDateTime(5, UnixTime(current_time));
                pstmt.setBigInt(6, steam_id);

                pstmt.executeUpdate();
                LogMessage("Insertion success where user ID >> " + steam_id, ISTR_MSG::good);
            }
        }
    }
    void DATABASE::IO_JSON_SQL::InsertAchievements(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt, const std::string& steam_id) {
        for (const std::unique_ptr<nlohmann::json>& json : data) {
            for (const auto& entry : *json) {
                const std::string  gameName = entry["gameName"];
                const std::string  apiname = entry["apiname"];
                const bool         achieved = (entry["achieved"] == 1);
                const long long    unlocktime = entry["unlocktime"];
                const double       percent = entry["percent"];
                const std::string  displayName = entry["displayName"];
                const std::string  description = entry["description"];

                pstmt.setBigInt(1, steam_id);
                pstmt.setString(2, gameName);
                pstmt.setString(3, apiname);
                pstmt.setString(4, displayName);
                pstmt.setString(5, description);
                pstmt.setDouble(6, percent);
                pstmt.setBoolean(7, achieved);
                pstmt.setDateTime(8, UnixTime(unlocktime));

                pstmt.executeUpdate();
                LogMessage("Insertion success where achievement ID >> " + apiname + " <<\tfor game >> " + gameName + " <<", ISTR_MSG::good);
            }
        }
    }
    void DATABASE::IO_JSON_SQL::ExtractAchivPlayer(const nlohmann::json& player, nlohmann::json& achievement_entry) {
        achievement_entry["apiname"] = player["apiname"];
        achievement_entry["achieved"] = player["achieved"];
        achievement_entry["unlocktime"] = player["unlocktime"];
    }
    void DATABASE::IO_JSON_SQL::ExtractAchivGlobal(const std::vector<std::unique_ptr<nlohmann::json>>& global, nlohmann::json& achievement_entry, const size_t index, const std::string& apiname) {
        auto global_ach = std::find_if(
            (*global[index])["achievementpercentages"]["achievements"].begin(),
            (*global[index])["achievementpercentages"]["achievements"].end(),
            [&apiname](const nlohmann::json& ach) { return ach["name"] == apiname; });


        if (global_ach != (*global[index])["achievementpercentages"]["achievements"].end()) {
            achievement_entry["percent"] = global_ach->at("percent");
        }
        else {
            achievement_entry["percent"] = 000.00;
        }
    }
    void DATABASE::IO_JSON_SQL::ExtractAchivSchema(const std::vector<std::unique_ptr<nlohmann::json>>& schema, nlohmann::json& achievement_entry, const size_t index, const std::string& apiname) {
        if ((*schema[index])["game"]["availableGameStats"]["achievements"].contains(apiname)) {

            achievement_entry["displayName"] = (*schema[index])["game"]["availableGameStats"]["achievements"][apiname]["displayName"];
            if ((*schema[index])["game"]["availableGameStats"]["achievements"][apiname].contains("description")) {
                achievement_entry["description"] = (*schema[index])["game"]["availableGameStats"]["achievements"][apiname]["description"];
            }
            else {
                achievement_entry["description"] = "DEFAULT";
            }
        }
        else {
            achievement_entry["displayName"] = "UNKNOWN";
            achievement_entry["description"] = "NOT PROVIDED";
        }
    }

}

