/*
This section is about where data is stored and container operations.

Although kind of useless mechanic, the approach is kind of like ESC with a manager class MDATA and the actual class DATA. Additonally
the class ACHIEVEMENTS which is three DATA's. The reason is the achievement data is divided into three parts by Steam API and the
data returned is a bit of a hard format to work with. So the solution is to treat it as a single object.

Importantly the API returns JSON objects and I store those JSONs on the heap via unique_ptr. The intent is to avoid moving/copying the
object and instead just move/copy the pointer. Obviously there must be at least one move at the very start.

Most logic is reliant on the ENTITY_TYPE enum. Because of how data which can exist in isolation, and achievements which can not behave,
there are two helper enums whos values are that of the corresponding main enum values.

IMPORTANT:: The program assumes your steam privacy settings are set to public. There will be no issues up untill SQL insertions, but SQL
tables require strict structure which assums some data like the date when you last played game X is public. Otherwise need to adjust
definition strings and insertion loops (not a big job) in Database.h
*/

#pragma once

#include "Inputs.h"
#include "json.hpp"

namespace ORDO {

    enum class ENTITY_TYPE {
        ach_schema,
        ach_global,
        ach_player,

        pl_summary,
        pl_games,
        pl_recent,
    };
    enum class PLAYER_TYPE {
        pl_summary = ENTITY_TYPE::pl_summary,
        pl_games = ENTITY_TYPE::pl_games,
        pl_recent = ENTITY_TYPE::pl_recent
    };
    enum class ACHIEVEMENT_TYPE {
        ach_schema = ENTITY_TYPE::ach_schema,
        ach_global = ENTITY_TYPE::ach_global,
        ach_player = ENTITY_TYPE::ach_player
    };


    typedef ENTITY_TYPE ENTITY;
    typedef ENTITY_TYPE URL_TYPE;
    typedef ENTITY_TYPE TABLE_TYPE;

    typedef ACHIEVEMENT_TYPE ACHIEVEMENT;
    typedef PLAYER_TYPE PLAYER;

    class MDATA;
    class ACHIEVEMENTS;

    /*
    As mentioned at the start, a wrapper class DATA, who has friends MDATA (manager), and ACHIEVEMENTS. DATA is the most basic one.
    The intent is to allow public creation of this class (though meaningless in this program), but more importantly to disable public
    manipulation of the container.
    Initialization has to be done in a deliberate way assigning type to the constructor.
    */
    class DATA {
        friend class MDATA;
        friend class ACHIEVEMENTS;

        std::vector<std::unique_ptr<nlohmann::json>> data;
        DATA(const ENTITY _type) :type(_type) {}
    public:

        //type with which script logic is determined
        const ENTITY type;


        //same as std::vector empty()
        bool Empty()const;


        //same as std::vector clear()
        void Clear();


        //uses std::move to add a JSON object. Will not add if null or empty JSON. The latter is done in a bad way
        //as the API can return an error message instead of empty IF the player has their data partially/fully private.
        bool Add(std::unique_ptr<nlohmann::json> _data);


        //dumps all the the data on the terminal
        void Display()const;


        //read access to the container, modification of ptr thanks to unique_ptr is not allowed
        //needed for file creation and SQL insertions
        const std::vector<std::unique_ptr<nlohmann::json>>& Read()const;


        ~DATA() = default;
    };



    /*
    Mostly same as DATA but since achievement data is broken apart by Steam API, and there is no unified way to actually tie
    them in a relational way because of API reasons out of my control, achievements are treated as a single entity.
    Initialization is the same as DATA with a diffirent helper enum.
    Unlike DATA, this type does not have an explicit type member, as it makes no sense anyway and kind of makes thing even more clunky.
    */
    class ACHIEVEMENTS {
        friend class MDATA;
        DATA global;
        DATA player;
        DATA schema;


        ACHIEVEMENTS(const ACHIEVEMENT for_global, const ACHIEVEMENT for_player, const ACHIEVEMENT for_schema) :
            global(static_cast<ENTITY>(for_global)),
            player(static_cast<ENTITY>(for_player)),
            schema(static_cast<ENTITY>(for_schema)) {}


    public:
        //if any of the three items passed are null or empty, the object will not be added
        //it works better here as some API are guaranteed to return empty if anything is wrong with the data
        bool Add(std::unique_ptr<nlohmann::json> for_global, std::unique_ptr<nlohmann::json> for_player, std::unique_ptr<nlohmann::json> for_schema);


        //calls DATA::Clear()
        void Clear();


        //calls DATA::Display()
        void Display(const ACHIEVEMENT type)const;


        //Uses the container which stores player achievement data to retrieve game names. Reason for using player not schema
        //is becasue schema is not guaranteed to give the correct name (may be empty or some dev-y type name)
        //returns a unique_ptr by copy, caller is owner
        std::unique_ptr<std::vector<std::string>> GetStoredNames()const;


        //returns DATA::Read, grants read access used in making files and SQL inserts
        const std::vector<std::unique_ptr<nlohmann::json>>& Read(const ACHIEVEMENT type)const;

        ~ACHIEVEMENTS() = default;
    private:
        ACHIEVEMENTS() = delete;
        ACHIEVEMENTS(const ACHIEVEMENTS&) = delete;
        ACHIEVEMENTS(ACHIEVEMENTS&&) = delete;
        ACHIEVEMENTS& operator=(const ACHIEVEMENTS&) = delete;
    };




    /*
    Acts as the single type to store all required data. Not much to it.
    */
    class MDATA {
        DATA player_summary;
        DATA player_recent;
        DATA player_games;
        ACHIEVEMENTS achievements;

    public:
        MDATA() :player_summary(ENTITY::pl_summary), player_recent(ENTITY::pl_recent), player_games(ENTITY::pl_games), achievements(ACHIEVEMENT::ach_global, ACHIEVEMENT::ach_player, ACHIEVEMENT::ach_schema) {}

        DATA& PlayerSummary();
        DATA& PlayerRecent();
        DATA& PlayerGames();
        ACHIEVEMENTS& Achievements();


        //clears all data in all containers
        void Clear();

    private:
        MDATA(const MDATA&) = delete;
        MDATA& operator=(const MDATA&) = delete;
        MDATA(MDATA&&)noexcept = delete;
    };


}