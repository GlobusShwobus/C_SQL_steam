#include "Pages.h"
#include "myhelpers.h"
#include "Inputs.h"
#include "logMessage.h"

namespace ORDO {

    void SteamPageMain(const STEAM& handle, MDATA& main_data) {

        const std::string header = std::string(STEAM_PAGE_MAIN) + std::string(STEAM_PAGE_MAIN_SELECTS);
        bool end_page = false;
        while (!end_page) {
            lazy::console_clear();
            lazy::console_title();
            const int select = Inputs::InputRange(1,5, header);

            switch (select) {
            case 1:        SteamPlayerPage(handle, main_data.PlayerSummary());     break;
            case 2:        SteamPlayerPage(handle, main_data.PlayerGames());       break;
            case 3:      SteamPlayerPage(handle, main_data.PlayerRecent());      break;
            case 4:       SteamAchievementPage(handle, main_data.Achievements()); break;
            case 5:       end_page = true;                                        break;
            }
        }
    }
    void SteamPlayerPage(const STEAM& handle, DATA& data) {

        const std::string header = std::string(STEAM_PLAYER_PAGE) + std::string(STEAM_PLAYER_PAGE_SELECTS);

        lazy::console_clear();
        lazy::console_title();
        const int select = Inputs::InputRange(1, 5, header);

        switch (select) {
        case 1:
            data.Clear(); //because they should actually just be singles but too lazy to make polymorph, and retarded
            handle.RequestPlayerRelated(data);
            break;
        case 2:
            data.Display();
            break;
        case 3:
            data.Clear();
            Logs::Add({level::INFO, "Container cleared" });
            break;
        case 4:
        {
            FILE_MANAGER fman(DATA_FOLDER);
            fman.MakeJSON(data.Read());
        }
        break;
        case 5:
            break;
        }
    }
    void SteamAchievementPage(const STEAM& handle, ACHIEVEMENTS& achievement_data) {

        const std::string header = std::string(STEAM_ACHIEVEMENT_PAGE) + std::string(STEAM_ACHIEVEMENT_PAGE_SELECTS);

        lazy::console_clear();
        lazy::console_title();
        const int select = Inputs::InputRange(1, 6, header);

        switch (select) {
        case 1:      handle.RequestAchievements(achievement_data);                              break;
        case 2:      handle.ReqeustAchievementsByChoice(achievement_data);                      break;
        case 3:    DisplayAchievementsPage(achievement_data);                                 break;
        case 4:     achievement_data.Clear(); Logs::Add({level::INFO,"container cleared"});                 break;
        case 5:     SaveAchievementsPage(achievement_data);                                    break;
        case 6:      break;
        }

    }
    void SQLPageMain(DATABASE& database, MDATA& data_col) {

        const std::string header = std::string(SQL_PAGE_MAIN) + std::string(SQL_PAGE_MAIN_SELECTS);
        bool end_page = false;
        while (!end_page) {
            lazy::console_clear();
            lazy::console_title();
            const int select = Inputs::InputRange(1, 4, header);

            switch (select) {
            case 1:       database.AssignWorkingSchema();          break;
            case 2:       database.CreateNewSchema();              break;
            case 3:     SQLInjectionsPage(database, data_col);   break;
            case 4:      end_page = true;                         break;
            }
        }
    }
    void SQLInjectionsPage(DATABASE& database, MDATA& main_data) {

        const std::string header = std::string(SQL_INJECTIONS_PAGE) + std::string(SQL_INJECTIONS_PAGE_SELECTS);
        lazy::console_clear();
        lazy::console_title();
        const int select = Inputs::InputRange(1, 5, header);

        switch (select) {
        case 1:     database.InsertPlayer(main_data.PlayerSummary());      break;
        case 2:     database.InsertPlayer(main_data.PlayerGames());        break;
        case 3:   database.InsertPlayer(main_data.PlayerRecent());       break;
        case 4:    database.InsertAchievements(main_data.Achievements()); break;
        case 5:break;
        }
    }


    void DisplayAchievementsPage(const ACHIEVEMENTS& achievement_data) {
        //add ability to pick from a list which to see
        lazy::console_clear();
        lazy::console_title();
        const int select = Inputs::InputRange(1, 4, PAGE_REQUEST_ACHIEVEMENT_DISPLAY);

        switch (select) {
        case 1:     achievement_data.Display(ACHIEVEMENT::ach_global);   break;
        case 2:     achievement_data.Display(ACHIEVEMENT::ach_player);   break;
        case 3:   achievement_data.Display(ACHIEVEMENT::ach_schema);   break;
        case 4:break;
        }
    }
    void SaveAchievementsPage(const ACHIEVEMENTS& achievement_data) {

        lazy::console_clear();
        lazy::console_title();
        const int select = Inputs::InputRange(1, 4, PAGE_REQUEST_ACHIEVEMENT_SAVEFILE);

        FILE_MANAGER fman(DATA_FOLDER);
        switch (select) {
        case 1:   fman.MakeManyJSON(achievement_data.Read(ACHIEVEMENT::ach_global), achievement_data.GetStoredNames());  break;
        case 2:   fman.MakeManyJSON(achievement_data.Read(ACHIEVEMENT::ach_player), achievement_data.GetStoredNames());  break;
        case 3: fman.MakeManyJSON(achievement_data.Read(ACHIEVEMENT::ach_schema), achievement_data.GetStoredNames());  break;
        case 4:break;
        }
    }

    std::unique_ptr<STEAM> SteamLogIn() {
        while (true) {
            lazy::console_clear();
            lazy::console_title();
            printf("\nVerify connection to steam web API or esc to exit\n=====================================================================================\n");

            const std::string steam_api_key = Inputs::InputStr("Enter Steam API key >>> ");
            const std::string steam_user_id = Inputs::InputStr("Enter Steam user id >>> ");

            try {
                return std::make_unique<STEAM>(steam_api_key, steam_user_id);
            }
            catch (const std::exception& e) {
                Logs::Add({level::BAD, e.what()});
            }
        }
        return nullptr;
    }
    std::unique_ptr<DATABASE> SQLLogIn(const STEAM& handle) {
        while (true) {
            lazy::console_clear();
            lazy::console_title();
            printf("\nVerify connection to SQL local server or esc to exit\n=====================================================================================\n");

            const std::string _server_name = Inputs::InputStr("Enter Server name >>> ");
            const std::string _server_password = Inputs::InputStr("Enter password >>> ");


            try {
                return std::make_unique<DATABASE>(_server_name, _server_password, handle.CurrentSteamWorkingID());
            }
            catch (const std::exception& e) {
                Logs::Add({ level::BAD, e.what() });
            }
        }
        return nullptr;
    }
}