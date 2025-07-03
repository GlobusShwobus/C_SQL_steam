#include "logMessage.h"
#include <memory>
namespace ORDO {
	
    std::vector<basic_log>& Logs::GetLogs() {
        static std::unique_ptr<std::vector<basic_log>> logs = std::make_unique<std::vector<basic_log>>();
        return *logs;
    }
    void Logs::Add(basic_log&& log) {
        GetLogs().emplace_back(std::move(log));
    }
    void Logs::Clear() {
        GetLogs().clear();
    }
    void Logs::ClearType(const level type) {
        auto& logs = Logs::GetLogs();
        logs.erase(std::remove_if(logs.begin(), logs.end(), [type](const basic_log& log) {
            return log.logLevel == type;
            }), logs.end());
    }
    const std::string& Logs::GetLastLog() {
        auto& logs = Logs::GetLogs();
        if (!logs.empty()) {
            return logs.back().info;
        }
        else {
            return noLogs;
        }
    }
    const std::string& Logs::GetLastLog(const level type) {
        auto& logs = Logs::GetLogs();
        const std::string* str = nullptr;

        for (const auto& log : logs) {
            if (log.logLevel == type) {
                str = &log.info;
            }
        }
        return str ? *str : noLogs;
    }
}