#pragma once
#include <string>
#include <vector>

#define NOMINMAX

namespace ORDO {

	namespace lazy {
        //not confirmed functions (subject to refactoring/removing)
        void RemoveSymbols(std::string& from, const std::string& symbols);
        void RemoveSymbols(std::string& from, const char symbol);
        void PrintList(const std::vector<std::pair<std::string, std::string>>& list);
        bool ListContains(const std::vector<std::string>& list, const std::string& lookFor);
        const std::string UnixTime(time_t unix_timestamp);
        std::string GetClipboard();//apperantly iostream and terminal can do basic ctrl+v itself... kind of depricated

        //confirmed functions
        void console_wait();
        void console_title();
        void console_clear();
	}

}