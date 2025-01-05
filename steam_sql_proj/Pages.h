#pragma once

#include "Steam.h"
#include "MakeFile.h"
#include "Database.h"

#define MAIN_PAGE "\n[1] Steam\n[2] MySQL\n[3] Exit\n\nInput >>> "

#define STEAM_PAGE_MAIN "Select which form of data to work with\n=====================================================================================\n\n"
#define STEAM_PAGE_MAIN_SELECTS "[1] Player summary\n[2] Player owned games\n[3] Player recent activity\n[4] Get achievements\n[5] Back\n\nInput >>> "

#define STEAM_PLAYER_PAGE "Decide further action. Requesting data a second time will overwrite the first entry\n=====================================================================================\n\n"
#define STEAM_PLAYER_PAGE_SELECTS "[1] Make web request\n[2] Show\n[3] Erase\n[4] Make JSON file\n[5] Back\n\nInput >>> "

#define STEAM_ACHIEVEMENT_PAGE "Decide further action. Requesting from a list does an extra API call to recieve a full\nlist tied to the user. Also due to Steam API setup, there are 3 calls made per game\nto retrieve all data.\n=====================================================================================\n\n"
#define STEAM_ACHIEVEMENT_PAGE_SELECTS "[1] Reqeust everything\n[2] Request from a list\n[3] Dump on screen\n[4] Erase everything\n[5] Make JSON files\n[6] Back\n\nInput >>> "

#define SQL_PAGE_MAIN "In order to make any SQL injections, the desired schema has to be set manually.\nCreating a new schema does not set it.\n=====================================================================================\n\n"
#define SQL_PAGE_MAIN_SELECTS "[1] Set schema\n[2] Create new schema\n[3] Injections\n[4] Back\n\nInput >>> "

#define SQL_INJECTIONS_PAGE "Tables are predefined in a relational way in which 'player_summary' is at the top of the hierarchy\n=====================================================================================\n\n"
#define SQL_INJECTIONS_PAGE_SELECTS "[1] Insert player summary\n[2] Insert game list\n[3] Insert recent activity log\n[4] Insert achievements\n[5] Back\n\nInput >>> "

#define MISC_PAGE "Misc...\n=====================================================================================\n\n"
#define MISC_PAGE_SELECTS "[1] Save Error log\n[2] Save default login\n[3] Reseat working schema\n[4] Reseat working steam id\n[5] Erase all heap data\n[6] Back\n\nInput >>> "


#define PAGE_REQUEST_ACHIEVEMENT_DISPLAY "The data to display is potentially too large to do all at once. Select one...\n\n[1] Global stats\n[2] Player stats\n[3] Schematics\n[4] Back\n\nInput >>> "
#define PAGE_REQUEST_ACHIEVEMENT_SAVEFILE "Choose which type of data to save\n\n[1] Global achievement data\n[2] Player achievement data\n[3] Game schema data\n[4] Back\n\nInput >>> "



namespace ORDO {

    //pages
    void SteamPageMain(const STEAM& handle, MDATA& main_data);
    void SteamPlayerPage(const STEAM& handle, DATA& specific_data);
    void SteamAchievementPage(const STEAM& handle, ACHIEVEMENTS& achievement_data);

    void SQLPageMain(DATABASE& database, MDATA& main_data);
    void SQLInjectionsPage(DATABASE& database, MDATA& main_data);

    void DisplayAchievementsPage(const ACHIEVEMENTS& achievement_data);
    void SaveAchievementsPage(const ACHIEVEMENTS& achievement_data);


    std::unique_ptr<STEAM> SteamLogIn();
    std::unique_ptr<DATABASE> SQLLogIn(const STEAM& handle);
}