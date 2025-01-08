#pragma once


//stdexcept

#include <memory>
#include <string>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <ctime>

//#include <ctime>

#define UK_ZERO 48           //unique key 0
#define UK_ONE 49            //unique key 1
#define UK_NINE 57           //unique key 9
#define UK_LOWER_A 97        //unique key a
#define UK_LOWER_Z 122       //unique key z
#define UK_CAPITAL_A 65      //unique key A
#define UK_CAPITAL_Z 90      //unique key Z
#define UK_SYMBOL_START 32   //unique key printable character start
#define UK_SYMBOL_END 126    //unique key printable character end

#define UK_UNDERSCORE 95     //unique key underscore                        
#define UK_SPACE 32          //unique key space 
#define UK_BACKSPACE 8       //unique key backspace
#define UK_COPY 22           //unique key ctrl + v
#define UK_ESCAPE 27         //unique key escape/esc
#define UK_RETURN 13         //unique key enter/return


#define SR_NULL 0            //select range 0
#define SR_ONE 1             //select range 1
#define SR_TWO 2             //select range 2
#define SR_THREE 3           //select range 3
#define SR_FOUR 4            //select range 4
#define SR_FIVE 5            //select range 5
#define SR_SIX 6             //select range 6
#define SR_SEVEN 7           //select range 7
#define SR_EIGHT 8           //select range 8
#define SR_NINE 9            //select range 9

/*
separation of concern:

function can not return 0 if premature carrige return is prssed
function can not return 0 if escape key is pressed
both above have to be resolved by a middleman
have functions that take string input, take ranged based number input, take any number input



logging a message and logging an error must be different
error has to ONLY be something that blocks any type of progress but can be resolved during runtime
messages can be any type of failures/successes/or any other feedback but not something which requires fixing code
errors should be stord permantly and loged into a file to keep track of things in need of fixing
messages can be ephemeral



in terms of access: inputs, loging and misc string manipulation should be divided up and type handling should not be user concern

other junk not sure
*/


namespace ORDO { //input string

    //input string
    class ISTR {

        enum class ASCII_TYPE {
            nums, numsNoZero, chars, numsAndChars, printables
        };

        static bool ASCIICheck(const ASCII_TYPE t, char c) {
            switch (t) {
            case ASCII_TYPE::nums:           return (c >= UK_ZERO && c <= UK_NINE);
            case ASCII_TYPE::numsNoZero:     return (c >= UK_ONE && c <= UK_NINE);
            case ASCII_TYPE::chars:          return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || c == UK_UNDERSCORE;
            case ASCII_TYPE::numsAndChars:   return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || (c >= UK_ZERO && c <= UK_NINE) || c == UK_UNDERSCORE;
            case ASCII_TYPE::printables:     return (c >= UK_SYMBOL_START && c <= UK_SYMBOL_END);
            }
            throw "Invalid argument";
        }
        static void CopiedContent(const ASCII_TYPE t, std::string& mutated) {
            std::string copy = HSTR::GetClipboard();//get all data in text form from clipboard

            copy.erase(std::remove_if(copy.begin(), copy.end(), [&t](const char c) { //remove every symbol which does not pass the check
                return !ASCIICheck(t, c);
                }), copy.end());


            for (const char c : copy) { //print on screen new data + store it
                _putch(c);
                mutated += c;
            }
        }
    public:

        static std::string IString() {
            static constexpr ASCII_TYPE restriction = ASCII_TYPE::numsAndChars;//shorthand to pass restructions for inputs (may be depricated in the future)

            std::string str;

            while (true || str.size() < 50) {
                const char c = _getch();

                if (c == UK_RETURN && !str.empty()) {
                    break;
                }
                if (ASCIICheck(restriction, c)) {
                    _putch(c);
                    str += c;
                }
                else if (c == UK_BACKSPACE && str.size() > 0) {
                    _putch('\b');
                    _putch(' ');
                    _putch('\b');
                    str.pop_back();
                }
                else if (c == UK_COPY) {
                    CopiedContent(restriction, str);
                }
                else if (c == UK_ESCAPE) {
                    str = "";
                }
            }

            return str;
        }



    };

    int Select(const unsigned int range, const INPUT_T restriction = INPUT_T::nums);

    namespace HSTR { //helper/junk string ops
        bool ContainsExit(const std::string& str);
        void RemoveSymbols(std::string& from, const std::string& symbols);
        void RemoveSymbols(std::string& from, const char symbol);

        void HandleSelectRange(const unsigned int range, std::string& number);
        void Wait();

        std::string GetClipboard();

        enum class MESSAGE_TYPE_INFO {
            good, fail, hard, other
        };

        const char* MessageColor(const MESSAGE_TYPE_INFO info);
    }

    typedef HSTR::MESSAGE_TYPE_INFO ISTR_ERROR;
    typedef HSTR::MESSAGE_TYPE_INFO ISTR_MSG;

    void LogError(const std::string& error, const ISTR_ERROR info);
    void LogError(const char* error, const ISTR_ERROR info);

    void LogMessage(const std::string& message, const ISTR_MSG info);
    void LogMessage(const char* message, const ISTR_MSG info);

    void PrintList(const std::vector<std::pair<std::string, std::string>>& list);

    namespace LOGSTR { // message/error log

        class ERROR_LOG final {
            static std::vector<std::string>* GetInstance() {
                static std::unique_ptr<std::vector<std::string>> error_log = std::make_unique<std::vector<std::string>>();
                return error_log.get();
            }
        public:
            static void Add(const std::string& what) {
                GetInstance()->push_back(what);
            }
            static void Add(const char* what) {
                GetInstance()->push_back(what);
            }
            static const std::vector<std::string>& Get() {
                return *GetInstance();
            }
        private:
            ERROR_LOG() = delete;
            ERROR_LOG(const ERROR_LOG&) = delete;
            ERROR_LOG& operator=(const ERROR_LOG&) = delete;
        };

        class MESSAGE_LOG {
            static std::vector<std::string>* GetInstance() {
                static std::unique_ptr<std::vector<std::string>> message_log = std::make_unique<std::vector<std::string>>();
                return message_log.get();
            }
        public:
            static void Add(const std::string& what) {
                GetInstance()->push_back(what);
            }
            static void Add(const char* what) {
                GetInstance()->push_back(what);
            }
            static void Flush() {
                for (const auto& each : *GetInstance()) {
                    printf("\n%s", each.c_str());
                }
                GetInstance()->clear();
            }


        private:
            MESSAGE_LOG() = delete;
            MESSAGE_LOG(const MESSAGE_LOG&) = delete;
            MESSAGE_LOG& operator=(const MESSAGE_LOG&) = delete;
        };
    }

    bool StrContains(const std::vector<std::string>& cont, const std::string& compared_to);
    const std::string UnixTime(time_t unix_timestamp);
    std::string EnterString(const INPUT_T type, const std::string& msg);
    int SelectOption(const unsigned int range, const std::string& msg);

    void Refresh();
    void PrintTitle();
    void ClearTerminal();
}
