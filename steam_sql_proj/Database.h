#pragma once

#include "Entity.h"

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

/*
This section is about inserting the aquired data into SQL.

The tables are predefined in a relational way. Player summary is the top table which describes the player. One level under it
in equal terms are player recently activity and player owned games list. The achievement table is related to player owned games list
(which itself is related to summary). To achieve this relation, the DATABASE class must also demand steam_id as a reference. This ID
is stored in the STEAM class.

Working_schema member is not quite what it seems. It just stores the name of the schema since connection established and disrupted all
the time, meaning it's better to have a function which reestablishes connection and calls setSchema function of mysql to work with a particular
database. Additionally, no operations can be done until working_schema is assigned using AssignWorkingSchema() function.

Similarly to STEAM, some functionality is further nested away into subclasses. This is again, mostly just for organization.
*/
namespace ORDO {

    class DATABASE {

        sql::mysql::MySQL_Driver* driver = nullptr;//is some sort of sql singleton, not my job to clean up
        const std::string         ip = "tcp://127.0.0.1:3306";
        std::string               server_name = "";
        std::string               server_password = "";
        std::string               working_schema = "";
        const std::string& steam_id;

        struct SETUP_STATEMENTS;
        class IO_JSON_SQL;


        //TABLE_TYPE is the same as ENTITY_TYPE and is used to create a table, it uses table
        void CreateTable(sql::Connection& conn, const TABLE_TYPE type)const;


        //checks if schema is set. if the SQL database is deleted during runtime, it does not automatically reset it but it won't crash
        //and is possible to reseat it using AssignWorkingSchema
        bool isWorkingSchema()const;


        //fixes input to be 0 indexed and returns if it is within range
        bool AssignSchemaIndexCheck(const int select, const size_t size)const;


        //uses MySQL information schemata to aquire a list of existsing schemas (databases) but excludes the meta ones like sys, information_schema etc
        std::unique_ptr<std::vector<std::string>> GetSchemaList()const;


        //returns a pointer to a sql::Connection but it also calls setSchema with working_schema
        std::unique_ptr<sql::Connection> Connect()const;
    public:

        DATABASE(const std::string& sName, const std::string& sPass, const std::string& _steam_id);

        //creates a new schema, does not assign it a working schema
        void CreateNewSchema()const;


        //sets the working schema which is always set when Connect() is called
        void AssignWorkingSchema();

        //inserts any of the 3 basic entity types into the sql tables which are defined as macros below
        void InsertPlayer(const DATA& data);


        //inserts achievement data into sql, it's logic is more stages and is broken apart in IO_JSON_SQL
        void InsertAchievements(const ACHIEVEMENTS& achs);

    };

    /*
    *functions which retrieve a corresponding create table or insert table statement defined by macros below as per entity type
    *each table has a specific name defined by DefinedNames
    *also a function which retrieves all tables inside the working_schema
    */
    struct DATABASE::SETUP_STATEMENTS {
        static const std::string CreateTableStmt(const TABLE_TYPE type);
        static const std::string InsertTableStmt(const TABLE_TYPE type);
        static const std::string DefinedNames(const TABLE_TYPE type);
        static std::unique_ptr<std::vector<std::string>> GetTablesList(sql::Connection& conn, const std::string& schema_name);
    };

    /*
    ill add this later
    */
    class DATABASE::IO_JSON_SQL {

        static void InsertSummary(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt);
        static void InsertGames(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt, const std::string& steam_id);
        static void InsertActivity(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt, const std::string& steam_id);
        static void InsertAchievements(const std::vector<std::unique_ptr<nlohmann::json>>& data, sql::PreparedStatement& pstmt, const std::string& steam_id);

        static void ExtractAchivPlayer(const nlohmann::json& player, nlohmann::json& achievement_entry);
        static void ExtractAchivGlobal(const std::vector<std::unique_ptr<nlohmann::json>>& global, nlohmann::json& achievement_entry, const size_t index, const std::string& apiname);
        static void ExtractAchivSchema(const std::vector<std::unique_ptr<nlohmann::json>>& schema, nlohmann::json& achievement_entry, const size_t index, const std::string& apiname);

    public:

        static void InsertData(const std::vector<std::unique_ptr<nlohmann::json>>& data, const std::string& steam_id, const TABLE_TYPE type, sql::PreparedStatement& pstmt);
        static std::vector<std::unique_ptr<nlohmann::json>> MergeAchievementData(const std::vector<std::unique_ptr<nlohmann::json>>& global, const std::vector<std::unique_ptr<nlohmann::json>>& player, const std::vector<std::unique_ptr<nlohmann::json>>& schema);
    };

}
#define CSUMMARY_TABLE "CREATE TABLE player_summary (id BIGINT PRIMARY KEY, persona_name VARCHAR(50), last_logoff DATE, time_created DATE, country_code VARCHAR(10));"
#define ISUMMARY_TABLE "INSERT INTO player_summary (id, persona_name, last_logoff, time_created, country_code) VALUES(?,?,?,?,?) ON DUPLICATE KEY UPDATE persona_name = VALUES(persona_name), last_logoff = VALUES(last_logoff), time_created = VALUES(time_created), country_code = VALUES(country_code);"
#define CGAMES_TABLE "CREATE TABLE player_owned_games (id VARCHAR(150), player_id BIGINT, game_code INT, playtime_minutes INT, last_played DATE, PRIMARY KEY (id, player_id), FOREIGN KEY (player_id) REFERENCES player_summary(id));"
#define IGAMES_TABLE "INSERT INTO player_owned_games (id, player_id, game_code, playtime_minutes, last_played) VALUES(?,?,?,?,?) ON DUPLICATE KEY UPDATE game_code = VALUES(game_code), playtime_minutes = VALUES(playtime_minutes), last_played = VALUES(last_played);"
#define CACTIVITY_TABLE "CREATE TABLE activity_log (entry INT PRIMARY KEY AUTO_INCREMENT, app_id INT, game_name VARCHAR(150), playtime_2weeks INT, playtime_forever INT, entry_date DATE, player_id BIGINT, FOREIGN KEY (player_id) REFERENCES player_summary(id));"
#define IACTIVITY_TABLE "INSERT INTO activity_log (app_id, game_name, playtime_2weeks, playtime_forever, entry_date, player_id) VALUES(?,?,?,?,?,?);"
#define CACHIEVEMENT_TABLE "CREATE TABLE achievements (player_id BIGINT, game_name VARCHAR(150), ach_id VARCHAR(150), ach_name VARCHAR(150), ach_descrip VARCHAR(255), global_percent DECIMAL(5,2), achieved TINYINT(1), unlock_time DATE, PRIMARY KEY (player_id, game_name, ach_id), FOREIGN KEY (game_name) REFERENCES player_owned_games(id), FOREIGN KEY (player_id) REFERENCES player_owned_games (player_id));"
#define IACHIEVEMENT_TABLE "INSERT INTO achievements (player_id, game_name, ach_id, ach_name, ach_descrip, global_percent, achieved, unlock_time) VALUES(?,?,?,?,?,?,?,?) ON DUPLICATE KEY UPDATE ach_name = VALUES(ach_name), ach_descrip = VALUES(ach_descrip), global_percent = VALUES(global_percent), achieved = VALUES(achieved), unlock_time = VALUES(unlock_time);"

