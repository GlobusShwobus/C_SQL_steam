#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace ORDO {

    enum level {
        INFO, WARN, BAD, FATAL, JUNK
    };
    struct basic_log {
        level severity = level::JUNK;
        std::string info;
    };

    class Logs {
        static constexpr std::string_view noLogs = "no logs";
    public:
        static std::vector<basic_log>& GetLogs();
        static void Add(basic_log&& what);
        static void Add(const level severity, std::string_view what);
        static void Clear();
        static void ClearType(const level severity);

        static std::string_view GetLastLog();
        static std::string_view GetLastLog(const level severity);
    };

}