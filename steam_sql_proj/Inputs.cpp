#include "Inputs.h"

namespace ORDO {
    std::string InputString(const INPUT_T restriction) {
        std::string input_str;

        for (char c = 0; c != UK_RETURN; c = _getch()) {
            if (HSTR::ValidInput(restriction, c)) {
                _putch(c);
                input_str += c;
            }
            else if (c == UK_BACKSPACE && input_str.size() > 0) {
                _putch('\b');
                _putch(' ');
                _putch('\b');
                input_str.pop_back();
            }
            else if (c == UK_COPY) {
                input_str += HSTR::HandleCopiedData(restriction);
            }
            else if (c == UK_ESCAPE) {
                input_str = UK_ESCAPE;
                break;
            }
        }
        return input_str;
    }

    int Select(const unsigned int range, const INPUT_T restriction) {
        std::string number;

        for (char c = NULL; c != UK_RETURN; c = _getch()) {
            if (HSTR::ValidInput(restriction, c)) {
                _putch(c);
                number += c;
                HSTR::HandleSelectRange(range, number);
            }
            else if (c == UK_BACKSPACE && number.size() > 0) {
                _putch('\b');
                _putch(' ');
                _putch('\b');
                number.pop_back();
            }
            else if (c == UK_COPY) {
                number += HSTR::HandleCopiedData(restriction);
                HSTR::HandleSelectRange(range, number);
            }
            else if (c == UK_ESCAPE) {
                return 0;
            }
        }

        if (number.empty()) {
            return 0;
        }

        return std::stoi(number);
    }
    bool HSTR::ValidInput(const INPUT_T restriction, char c) {
        switch (restriction) {
        case INPUT_T::nums:       return (c >= UK_ZERO && c <= UK_NINE);
        case INPUT_T::numsNoZero: return (c >= UK_ONE && c <= UK_NINE);
        case INPUT_T::chars:      return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || c == UK_UNDERSCORE;
        case INPUT_T::numchars:   return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || (c >= UK_ZERO && c <= UK_NINE) || c == UK_UNDERSCORE;
        case INPUT_T::printables: return (c >= UK_SYMBOL_START && c <= UK_SYMBOL_END);
        }
        return false;
    }
    bool HSTR::ContainsExit(const std::string& str) {
        return str.find(UK_ESCAPE) != std::string::npos;
    }
    void HSTR::RemoveSymbols(std::string& from, const INPUT_T restriction) {
        from.erase(std::remove_if(from.begin(), from.end(), [restriction](const char c) {
            return !ValidInput(restriction, c);
            }), from.end());
    }
    void HSTR::RemoveSymbols(std::string& from, const std::string& symbols) {
        from.erase(std::remove_if(from.begin(), from.end(), [&symbols](const char c) {
            return symbols.find(c) != std::string::npos;
            }), from.end());
    }
    void HSTR::RemoveSymbols(std::string& from, const char symbol) {
        from.erase(std::remove_if(from.begin(), from.end(), [symbol](const char c) {
            return symbol == c;
            }), from.end());
    }

    std::string HSTR::HandleCopiedData(const INPUT_T restriction) {
        std::string copy = HSTR::GetClipboard();
        RemoveSymbols(copy, restriction);

        for (const char c : copy) {
            _putch(c);
        }
        return copy;
    }
    void HSTR::HandleSelectRange(const unsigned int range, std::string& number) {
        try {
            int val = std::stoi(number);
            if (val > range) {
                for (char c : number) {
                    _putch('\b');
                    _putch(' ');
                    _putch('\b');
                }
                number = std::to_string(range);

                for (char c : number) {
                    _putch(c);
                }
            }
        }
        catch (const std::exception& e) {
            for (char c : number) {
                _putch('\b');
                _putch(' ');
                _putch('\b');
            }
            number = std::to_string(range);

            for (char c : number) {
                _putch(c);
            }
        }
    }

    void HSTR::Wait() {
        printf("\nPress a key...");
        char empty_input = _getch();
    }
    std::string HSTR::GetClipboard() {
        if (!OpenClipboard(nullptr)) {
            return "";
        }
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData == nullptr) {
            CloseClipboard();
            return "";
        }

        char* copy_stream = static_cast<char*>(GlobalLock(hData));
        std::string text;

        if (copy_stream) {
            text = copy_stream;
        }

        GlobalUnlock(hData);
        CloseClipboard();

        return text;
    }
    const char* HSTR::MessageColor(const MESSAGE_TYPE_INFO info) {
        switch (info) {
        case MESSAGE_TYPE_INFO::good:  return "\033[32m";
        case MESSAGE_TYPE_INFO::fail:  return "\033[33m";
        case MESSAGE_TYPE_INFO::hard:  return "\033[31m";
        case MESSAGE_TYPE_INFO::other: return "\033[34m";
        }
        return "\033[97m";
    }
    void LogError(const std::string& error, const ISTR_ERROR info) {
        LOGSTR::ERROR_LOG::Add(error);
        LOGSTR::MESSAGE_LOG::Add(error);
    }
    void LogError(const char* error, const ISTR_ERROR info) {
        LOGSTR::ERROR_LOG::Add(error);
        LOGSTR::MESSAGE_LOG::Add(error);
    }
    void LogMessage(const std::string& message, const ISTR_MSG info) {
        LOGSTR::MESSAGE_LOG::Add(message);
    }
    void LogMessage(const char* message, const ISTR_MSG info) {
        LOGSTR::MESSAGE_LOG::Add(message);
    }

    void PrintList(const std::vector<std::pair<std::string, std::string>>& list) {
        size_t i = 1;

        for (const auto& each : list) {
            printf("\n[%zu] %s", i++, each.second.c_str());
        }
    }
    bool StrContains(const std::vector<std::string>& cont, const std::string& compared_to) {
        return std::find(cont.begin(), cont.end(), compared_to) != cont.end();
    }
    const std::string UnixTime(time_t unix_timestamp) {

        char time_buf[80];
        struct tm ts;
        localtime_s(&ts, &unix_timestamp);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &ts);

        return std::string(time_buf);
    }
    std::string EnterString(const INPUT_T type, const std::string& msg) {
        printf("\n%s", msg.c_str());
        std::string input = InputString(type);
        return input;
    }
    int SelectOption(const unsigned int range, const std::string& msg) {
        printf("\n%s", msg.c_str());
        int input = Select(range, INPUT_T::numsNoZero);

        return input;
    }


    void Refresh() {
        ClearTerminal();
        PrintTitle();
        LOGSTR::MESSAGE_LOG::Flush();
    }

    void PrintTitle() {
        printf("\n%s", R"( @@@@@@   @@@@@@@   @@@@@@@    @@@@@@   
@@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  
@@!  @@@  @@!  @@@  @@!  @@@  @@!  @@@  
!@!  @!@  !@!  @!@  !@!  @!@  !@!  @!@  
@!@  !@!  @!@!!@!   @!@  !@!  @!@  !@!  
!@!  !!!  !!@!@!    !@!  !!!  !@!  !!!  
!!:  !!!  !!: :!!   !!:  !!!  !!:  !!!  
:!:  !:!  :!:  !:!  :!:  !:!  :!:  !:!  
::::: ::  ::   :::   :::: ::  ::::: ::  
 : :  :    :   : :  :: :  :    : :  :)");

        printf("%s", "\n\n=====================================================================================");
    }
    void ClearTerminal() {
        system("cls");
    }
}
