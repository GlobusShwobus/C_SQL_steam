#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  


#include"web_request.h"
#include "json.hpp"
#include "Inputs.h"
#include "myHelpers.h"
#include "steamURlEditor.h"

using namespace ORDO;
/*
SAVE LOICENSE PATH IN JSON (maybe other shit too, pobably not api key and sql psswords tho)
*/


void textFileTest() {
    Request test;

    std::string result;
    test.setLoicence("C:/Users/ADMIN/Desktop/tools/cacert.pem", &result);//simply using "cacert.pem" does not work, whatever...

    std::cout << result << "\n";

    std::string textFileMessage;

    auto textFile = test.request("http://127.0.0.1:8000/Desktop/testLocal/textFile.txt", &textFileMessage);

    std::cout << textFileMessage<<"\n";

    if (textFile.get()->isSuccess) {

        std::cout << textFile.get()->contentType<<"\n";

        if (textFile.get()->contentType.find("text/") != std::string::npos) {
            auto& toStringMePlease = textFile.get()->data;
            std::string textFileData(toStringMePlease.begin(), toStringMePlease.end());
            std::cout << textFileData;
        }

    }
}
void jsonFileTest() {
    Request test;

    std::string result;
    test.setLoicence("C:/Users/ADMIN/Desktop/tools/cacert.pem", &result);//simply using "cacert.pem" does not work, whatever...

    std::cout << result << "\n";

    std::string textFileMessage;

    auto textFile = test.request("http://127.0.0.1:8000/Desktop/testLocal/jsonFile.json", &textFileMessage);

    std::cout << textFileMessage<<"\n";

    if (textFile.get()->isSuccess) {

        std::cout << textFile.get()->contentType<<"\n";

        if (textFile.get()->contentType.find("application/json") != std::string::npos) {
            auto& jsonMePlease = textFile.get()->data;

            nlohmann::json jsonFileData = nlohmann::json::parse(jsonMePlease.begin(), jsonMePlease.end());
            std::cout << jsonFileData.dump(4);
        }

    }
}
void pngFileTest() {
    Request test;

    std::string result;
    test.setLoicence("C:/Users/ADMIN/Desktop/tools/cacert.pem", &result);//simply using "cacert.pem" does not work, whatever...

    std::cout << result << "\n";

    std::string textFileMessage;

    auto textFile = test.request("http://127.0.0.1:8000/Desktop/testLocal/pngFile.png", &textFileMessage);

    std::cout << textFileMessage << "\n";

    if (textFile.get()->isSuccess) {

        std::cout << textFile.get()->contentType << "\n";

        if (textFile.get()->contentType.find("image/") != std::string::npos) {
            auto& imageMePlease = textFile.get()->data;

            for (char c : imageMePlease) {
                std::cout << c;//gibberish but works, in the future how tf do i inject it into disk?
            }
        }

    }
}
void testInputsString1() {
    std::cout << "enter: ";
    std::string str1 = Inputs::InputStr();
    std::cout << str1;
}
void testInputInt1() {
    std::cout << "enter: ";
    int n = Inputs::InputInt();
    std::cout << n;
}
void testInputInt2() {
    bool loop = true;

    while (loop) {

        try {
            std::cout << "enter: ";
            int n = Inputs::InputInt();
            std::cout << n;
            loop = false;
        }
        catch (std::exception& e) {
            std::cout << e.what() << "\n";;
        }

    }
}
void testInputFloat() {
    bool loop = true;

    while (loop) {

        try {
            std::cout << "enter: ";
            float n = Inputs::InputFloat();
            std::cout << n;
            loop = false;
        }
        catch (std::exception& e) {
            std::cout << e.what() << "\n";
        }

    }
}
void testInputRange() {
    bool loop = true;

    while (loop) {

        try {
            std::cout << "enter: ";
            int n = Inputs::InputRangeInt(5, 50);
            std::cout << n;
            loop = false;
        }
        catch (std::exception& e) {
            std::cout << e.what() << "\n";
        }

    }
}
void testtitle() {
    while (true) {
        lazy::console_title();
        std::cout<<"\n";
        testInputsString1();
        std::cout << "\n";
        lazy::console_wait();
        lazy::console_clear();
    }
}

void teststeamurlThing_part2(const std::unique_ptr<Request::ResoponseBuffer>& poop) {
    if (poop.get()->isSuccess) {

        std::cout << poop.get()->contentType << "\n";

        if (poop.get()->contentType.find("application/json") != std::string::npos) {
            auto& jsonMePlease = poop.get()->data;

            nlohmann::json jsonFileData = nlohmann::json::parse(jsonMePlease.begin(), jsonMePlease.end());
            std::cout << jsonFileData.dump(4);
        }

    }
}
void teststeamUrlThing(){
    lazy::console_title();
    std::cout << "\nenter api key: ";
    Inputs::clearInputBuffer();
    std::string apikey = Inputs::InputStr();
    std::cout << "\nenter userid key: ";
    Inputs::clearInputBuffer();
    std::string userid = Inputs::InputStr();

    SteamURLEditor urlthing(apikey, userid);
    Request request("C:/Users/ADMIN/Desktop/tools/cacert.pem");

    std::cout << "\nsteam user key < " << urlthing.getAPIKey() << " >\tsteam user id < " << urlthing.getUserID() << " >";
    std::cout << "\n pick one:\n1.summary\n2.games\n3.recently played\n4.achievemnt player\n5.achievemnt global\n6.achievement schema\nenter: ";
    int pick = Inputs::InputRangeInt(1, 6);
   
    SAPI_FUNCTIONS func;
    switch (pick) {
    case 1: func = SAPI_FUNCTIONS::SUMMARY;            break;
    case 2: func = SAPI_FUNCTIONS::GAMES;              break;
    case 3: func = SAPI_FUNCTIONS::RECENTLY_PLAYED;    break;
    case 4: func = SAPI_FUNCTIONS::ACHIEVEMENTS_PLAYER;break;
    case 5: func = SAPI_FUNCTIONS::ACHIEVEMENTS_GLOBAL;break;
    case 6: func = SAPI_FUNCTIONS::ACHIEVEMENTS_SCHEMA;break;
    }

    if (pick > 3 && pick < 7) {
        std::cout << "enter game id: ";
        Inputs::clearInputBuffer();
        std::string gameid = Inputs::InputStr();
        std::string url = urlthing.get_API_URL(func, gameid);
        auto data = request.request(url);
        teststeamurlThing_part2(data);
    }
    else {
        std::string url = urlthing.get_API_URL(func);
        auto data = request.request(url);
        teststeamurlThing_part2(data);
    }


}


int main() {
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);


        lazy::console_clear();
        teststeamUrlThing();
        lazy::console_wait();



    _CrtDumpMemoryLeaks();
    
    return 0;
}

/*
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
        lazy::console_clear();
        lazy::console_title();

        const int select = Inputs::InputRange(1, 3, MAIN_PAGE);

        switch (select) {
        case 1:
            SteamPageMain(*handle, data_col);
            break;
        case 2:
            SQLPageMain(*database, data_col);
            break;
        case 3:
            end_session = true;
            break;
        }
    }

    return 0;
}
*/