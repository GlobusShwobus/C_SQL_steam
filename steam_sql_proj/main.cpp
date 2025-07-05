
#include"web_request.h"
#include "json.hpp"
#include "Inputs.h"
#include "myHelpers.h"

using namespace ORDO;


void textFileTest() {
    Request test;

    std::string result;
    test.setLoicence("C:/Users/ADMIN/Desktop/tools/cacert.pem", &result);//simply using "cacert.pem" does not work, whatever...

    std::cout << result << "\n";

    std::string textFileMessage;

    auto textFile = test.request("http://127.0.0.1:8000/Desktop/testLocal/textFile.txt", &textFileMessage);

    std::cout << textFileMessage<<"\n";

    if (textFile.get()->success) {

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

    if (textFile.get()->success) {

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

    if (textFile.get()->success) {

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
            int n = Inputs::InputRange(5, 50);
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

int main() {
    testtitle();

    
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