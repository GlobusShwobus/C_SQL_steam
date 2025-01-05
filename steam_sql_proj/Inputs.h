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


namespace ORDO { //input string

    enum class INPUT_T {
        nums, numsNoZero, chars, numchars, printables
    };

    std::string InputString(const INPUT_T restriction);
    int Select(const unsigned int range, const INPUT_T restriction = INPUT_T::nums);

    namespace HSTR { //helper/junk string ops
        bool ValidInput(const INPUT_T restriction, char c);
        bool ContainsExit(const std::string& str);
        void RemoveSymbols(std::string& from, const INPUT_T restriction);
        void RemoveSymbols(std::string& from, const std::string& symbols);
        void RemoveSymbols(std::string& from, const char symbol);

        std::string HandleCopiedData(const INPUT_T restriction);
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
