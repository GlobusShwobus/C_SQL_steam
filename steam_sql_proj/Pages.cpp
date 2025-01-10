#include "Pages.h"


namespace ORDO {

    void SteamPageMain(const STEAM& handle, MDATA& main_data) {

        const std::string header = std::string(STEAM_PAGE_MAIN) + std::string(STEAM_PAGE_MAIN_SELECTS);
        bool end_page = false;
        while (!end_page) {
            TRefresh();
            const int select = ISTR::InputRange(SR_FIVE, header);

            switch (select) {
            case SR_ONE:        SteamPlayerPage(handle, main_data.PlayerSummary());     break;
            case SR_TWO:        SteamPlayerPage(handle, main_data.PlayerGames());       break;
            case SR_THREE:      SteamPlayerPage(handle, main_data.PlayerRecent());      break;
            case SR_FOUR:       SteamAchievementPage(handle, main_data.Achievements()); break;
            case SR_FIVE:       end_page = true;                                        break;
            }
        }
    }
    void SteamPlayerPage(const STEAM& handle, DATA& data) {

        const std::string header = std::string(STEAM_PLAYER_PAGE) + std::string(STEAM_PLAYER_PAGE_SELECTS);

        TRefresh();
        const int select = ISTR::InputRange(SR_FIVE, header);

        switch (select) {
        case SR_ONE:
            data.Clear(); //because they should actually just be singles but too lazy to make polymorph, and retarded
            handle.RequestPlayerRelated(data);
            break;
        case SR_TWO:
            data.Display();
            break;
        case SR_THREE:
            data.Clear();
            LogMessage("Container cleared");
            break;
        case SR_FOUR:
        {
            FILE_MANAGER fman(DATA_FOLDER);
            fman.MakeJSON(data.Read());
        }
        break;
        case SR_FIVE:
            break;
        }
    }
    void SteamAchievementPage(const STEAM& handle, ACHIEVEMENTS& achievement_data) {

        const std::string header = std::string(STEAM_ACHIEVEMENT_PAGE) + std::string(STEAM_ACHIEVEMENT_PAGE_SELECTS);

        TRefresh();
        const int select = ISTR::InputRange(SR_SIX, header);

        switch (select) {
        case SR_ONE:      handle.RequestAchievements(achievement_data);                              break;
        case SR_TWO:      handle.ReqeustAchievementsByChoice(achievement_data);                      break;
        case SR_THREE:    DisplayAchievementsPage(achievement_data);                                 break;
        case SR_FOUR:     achievement_data.Clear(); LogMessage("Container cleared");                 break;
        case SR_FIVE:     SaveAchievementsPage(achievement_data);                                    break;
        case SR_SIX:      break;
        }

    }
    void SQLPageMain(DATABASE& database, MDATA& data_col) {

        const std::string header = std::string(SQL_PAGE_MAIN) + std::string(SQL_PAGE_MAIN_SELECTS);
        bool end_page = false;
        while (!end_page) {
            TRefresh();
            int select = ISTR::InputRange(SR_FOUR, header);

            switch (select) {
            case SR_ONE:       database.AssignWorkingSchema();          break;
            case SR_TWO:       database.CreateNewSchema();              break;
            case SR_THREE:     SQLInjectionsPage(database, data_col);   break;
            case SR_FOUR:      end_page = true;                         break;
            }
        }
    }
    void SQLInjectionsPage(DATABASE& database, MDATA& main_data) {

        const std::string header = std::string(SQL_INJECTIONS_PAGE) + std::string(SQL_INJECTIONS_PAGE_SELECTS);
        TRefresh();
        const int select = ISTR::InputRange(SR_FIVE, header);

        switch (select) {
        case SR_ONE:     database.InsertPlayer(main_data.PlayerSummary());      break;
        case SR_TWO:     database.InsertPlayer(main_data.PlayerGames());        break;
        case SR_THREE:   database.InsertPlayer(main_data.PlayerRecent());       break;
        case SR_FOUR:    database.InsertAchievements(main_data.Achievements()); break;
        case SR_FIVE:break;
        }
    }


    void DisplayAchievementsPage(const ACHIEVEMENTS& achievement_data) {
        //add ability to pick from a list which to see
        TRefresh();

        const int select = ISTR::InputRange(SR_FOUR, PAGE_REQUEST_ACHIEVEMENT_DISPLAY);

        switch (select) {
        case SR_ONE:     achievement_data.Display(ACHIEVEMENT::ach_global);   break;
        case SR_TWO:     achievement_data.Display(ACHIEVEMENT::ach_player);   break;
        case SR_THREE:   achievement_data.Display(ACHIEVEMENT::ach_schema);   break;
        case SR_FOUR:break;
        }
    }
    void SaveAchievementsPage(const ACHIEVEMENTS& achievement_data) {
        TRefresh();
        const int select = ISTR::InputRange(SR_FOUR, PAGE_REQUEST_ACHIEVEMENT_SAVEFILE);
        FILE_MANAGER fman(DATA_FOLDER);
        switch (select) {
        case SR_ONE:   fman.MakeManyJSON(achievement_data.Read(ACHIEVEMENT::ach_global), achievement_data.GetStoredNames());  break;
        case SR_TWO:   fman.MakeManyJSON(achievement_data.Read(ACHIEVEMENT::ach_player), achievement_data.GetStoredNames());  break;
        case SR_THREE: fman.MakeManyJSON(achievement_data.Read(ACHIEVEMENT::ach_schema), achievement_data.GetStoredNames());  break;
        case SR_FOUR:break;
        }
    }

    std::unique_ptr<STEAM> SteamLogIn() {
        while (true) {
            TRefresh();
            printf("\nVerify connection to steam web API or esc to exit\n=====================================================================================\n");

            const std::string steam_api_key = ISTR::InputStr("Enter Steam API key >>> ");
            const std::string steam_user_id = ISTR::InputStr("Enter Steam user id >>> ");

            try {
                return std::make_unique<STEAM>(steam_api_key, steam_user_id);
            }
            catch (const std::exception& e) {
                LogError(e.what());
            }
        }
        return nullptr;
    }
    std::unique_ptr<DATABASE> SQLLogIn(const STEAM& handle) {
        while (true) {
            TRefresh();
            printf("\nVerify connection to SQL local server or esc to exit\n=====================================================================================\n");

            const std::string _server_name = ISTR::InputStr("Enter Server name >>> ");
            const std::string _server_password = ISTR::InputStr("Enter password >>> ");


            try {
                return std::make_unique<DATABASE>(_server_name, _server_password, handle.CurrentSteamWorkingID());
            }
            catch (const std::exception& e) {
                LogError(e.what());
            }
        }
        return nullptr;
    }
}