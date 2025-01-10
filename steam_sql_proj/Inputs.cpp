#include "Inputs.h"

namespace ORDO {


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //  input string stuff

    bool ISTR::ASCIICheck(const ASCII_TYPE t, char c) {
        switch (t) {
        case ASCII_TYPE::nums:           return (c >= UK_ZERO && c <= UK_NINE);
        case ASCII_TYPE::chars:          return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || c == UK_UNDERSCORE;
        case ASCII_TYPE::numsAndChars:   return (c >= UK_LOWER_A && c <= UK_LOWER_Z) || (c >= UK_CAPITAL_A && c <= UK_CAPITAL_Z) || (c >= UK_ZERO && c <= UK_NINE) || c == UK_UNDERSCORE;
        case ASCII_TYPE::printables:     return (c >= UK_SYMBOL_START && c <= UK_SYMBOL_END);
        }
        throw "Invalid argument";
    }
    void ISTR::CopiedContent(const ASCII_TYPE t, std::string& mutate, const int max_size) {
        std::string copy = MSTR::GetClipboard();//get all data in text form from clipboard

        copy.erase(std::remove_if(copy.begin(), copy.end(), [&t](const char c) { //remove every symbol which does not pass the check
            return !ASCIICheck(t, c);
            }), copy.end());


        mutate += copy;

        if (mutate.size() > max_size) {
            mutate.resize(max_size);
        }
    }
    void ISTR::TerminalPrint(const std::string& str) {
        for (int i = 0; i < str.size(); ++i) {
            _putch('\b');
            _putch(' ');
            _putch('\b');
        }
        for (char each : str) {
            _putch(each);
        }
    }
    std::string ISTR::InputStr() {
        static constexpr ASCII_TYPE restriction = ASCII_TYPE::numsAndChars;//shorthand to pass restructions for inputs (may be depricated in the future)
        static constexpr short max_input_len = 30;
        std::string str;

        while (str.size() < max_input_len) {
            const char c = _getch();

            if (c == UK_RETURN && !str.empty()) {
                break;
            }
            if (ASCIICheck(restriction, c)) {
                str += c;
            }
            else if (c == UK_BACKSPACE && !str.empty()) {
                str.pop_back();
            }
            else if (c == UK_COPY) {
                CopiedContent(restriction, str, max_input_len);
            }
            else if (c == UK_ESCAPE) {
                return "";
            }

            TerminalPrint(str);
        }

        return str;
    }
    int ISTR::InputNum() {
        static constexpr ASCII_TYPE restriction = ASCII_TYPE::nums;//shorthand to pass restructions for inputs (may be depricated in the future)
        static constexpr short max_num_digits = 9;
        std::string str;

        while (str.size() < max_num_digits) {
            const char c = _getch();

            if (c == UK_RETURN && !str.empty()) {
                break;
            }
            if (ASCIICheck(restriction, c)) {
                str += c;
            }
            else if (c == UK_BACKSPACE && !str.empty()) {
                str.pop_back();
            }
            else if (c == UK_COPY) {
                CopiedContent(restriction, str, max_num_digits);
            }
            else if (c == UK_ESCAPE) {
                return -1;
            }
            TerminalPrint(str);
        }

        return std::stoi(str);
    }

    int ISTR::InputRange(const unsigned int range) {
        static constexpr ASCII_TYPE restriction = ASCII_TYPE::nums;//shorthand to pass restructions for inputs (may be depricated in the future)
        std::string str;


        while (true) {
            const char c = _getch();

            if (c == UK_RETURN && !str.empty()) {
                break;
            }
            if (ASCIICheck(restriction, c)) {
                if (str.empty() && c == UK_ZERO) {//first number can't be 0
                    continue;
                }
                if (std::stoi(str += c) > range) {
                    str.pop_back();
                }
            }
            else if (c == UK_BACKSPACE && str.size() > 0) {
                str.pop_back();
            }
            else if (c == UK_ESCAPE) {
                return -1;
            }
            TerminalPrint(str);
        }

        return std::stoi(str);
    }
    std::string ISTR::InputStr(const std::string& msg) {
        printf("\n%s", msg.c_str());
        return InputStr();
    }
    int ISTR::InputNum(const std::string& msg) {
        printf("\n%s", msg.c_str());
        return InputNum();
    }
    int ISTR::InputRange(const unsigned int range, const std::string& msg) {
        printf("\n%s", msg.c_str());
        return InputRange(range);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   logging stuff

    void LogError(const std::string& error) {
        ERROR_LOG::Add(error);
        MESSAGE_LOG::Add(error);
    }
    void LogMessage(const std::string& message) {
        MESSAGE_LOG::Add(message);
    }
    std::vector<std::string>* ERROR_LOG::GetInstance() {
        static std::unique_ptr<std::vector<std::string>> error_log = std::make_unique<std::vector<std::string>>();
        return error_log.get();
    }
    void ERROR_LOG::Add(const std::string& what) {
        GetInstance()->push_back(what);
    }
    void ERROR_LOG::Add(const char* what) {
        GetInstance()->push_back(what);
    }
    const std::vector<std::string>& ERROR_LOG::Get() {
        return *GetInstance();
    }
    std::vector<std::string>* MESSAGE_LOG::GetInstance() {
        static std::unique_ptr<std::vector<std::string>> message_log = std::make_unique<std::vector<std::string>>();
        return message_log.get();
    }
    void MESSAGE_LOG::Add(const std::string& what) {
        GetInstance()->push_back(what);
    }
    void MESSAGE_LOG::Add(const char* what) {
        GetInstance()->push_back(what);
    }
    void MESSAGE_LOG::Flush() {
        for (const auto& each : *GetInstance()) {
            printf("\n%s", each.c_str());
        }
        GetInstance()->clear();
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   string manipulation stuff

    void MSTR::RemoveSymbols(std::string& from, const std::string& symbols) {
        from.erase(std::remove_if(from.begin(), from.end(), [&symbols](const char c) {
            return symbols.find(c) != std::string::npos;
            }), from.end());
    }
    void MSTR::RemoveSymbols(std::string& from, const char symbol) {
        from.erase(std::remove_if(from.begin(), from.end(), [symbol](const char c) {
            return symbol == c;
            }), from.end());
    }
    void MSTR::PrintList(const std::vector<std::pair<std::string, std::string>>& list) {
        size_t i = 1;

        for (const auto& each : list) {
            printf("\n[%zu] %s", i++, each.second.c_str());
        }
    }
    bool MSTR::ListContains(const std::vector<std::string>& cont, const std::string& compared_to) {
        return std::find(cont.begin(), cont.end(), compared_to) != cont.end();
    }
    const std::string MSTR::UnixTime(time_t unix_timestamp) {

        char time_buf[80];
        struct tm ts;
        localtime_s(&ts, &unix_timestamp);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &ts);

        return std::string(time_buf);
    }
    std::string MSTR::GetClipboard() {
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
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   terminal related


    void TWait() {
        printf("\nPress a key...");
        char empty_input = _getch();
    }
    void TRefresh() {
        TClearTerminal();
        TPrintTitle();
        MESSAGE_LOG::Flush();
    }

    void TPrintTitle() {
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
    void TClearTerminal() {
        system("cls");
    }
}
