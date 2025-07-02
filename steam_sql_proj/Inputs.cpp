#include "Inputs.h"

namespace ORDO {


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //  input string stuff

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
