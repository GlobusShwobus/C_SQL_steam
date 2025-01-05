#include "Pages.h"


int main() {

    using namespace ORDO;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    std::unique_ptr<STEAM> handle = SteamLogIn();
    if (handle == nullptr) {
        printf("\nConnection proccess aborted. Can not continue without it.");
    }

    std::unique_ptr<DATABASE> database = SQLLogIn(*handle);
    if (database == nullptr) {
        printf("\nConnection proccess aborted. Can not continue without it.");
    }

    MDATA data_col;
    bool end_session = false;

    while (!end_session) {
        Refresh();

        const int select = SelectOption(SR_THREE, MAIN_PAGE);

        switch (select) {
        case SR_ONE:
            SteamPageMain(*handle, data_col);
            break;
        case SR_TWO:
            SQLPageMain(*database, data_col);
            break;
        case SR_THREE:
            end_session = true;
            break;
        }
    }

    return 0;
}