#pragma once
#include <string>
#include <vector>

namespace ORDO {
    namespace ERROR_LOG {
        static std::vector<std::string>& GetLogs();
        static const std::string& GetLastLog();
        static void PopBack();
        static void Add(const std::string& what);
        static void Add(const char* what);
        static void Flush();
    }
    namespace MESSAGE_LOG {
        static std::vector<std::string>& GetLogs();
        static const std::string& GetLastLog();
        static void PopBack();
        static void Add(const std::string& what);
        static void Add(const char* what);
        static void Flush();
    }
}