#pragma once
#include <string>
#include <vector>

namespace ORDO {

    enum level {
        INFO, WARN, BAD, FATAL, JUNK
    };
    struct basic_log {
        level logLevel = level::JUNK;
        std::string info;
        basic_log() = delete;
        basic_log(const level type, const std::string& str) :logLevel(type), info(str) {}
        basic_log(const level type, std::string&& str) :logLevel(type), info(std::move(str)) {}
    };

    class Logs {
        static constexpr std::string noLogs = "no logs";
    public:
        static std::vector<basic_log>& GetLogs();
        static void Add(basic_log&& log);
        static void Clear();
        static void ClearType(const level type);
        static const std::string& GetLastLog();
        static const std::string& GetLastLog(const level type);
    };

}