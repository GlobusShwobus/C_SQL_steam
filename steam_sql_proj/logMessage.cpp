#include "logMessage.h"
#include <memory>
namespace ORDO {
	
    std::vector<basic_log>& Logs::GetLogs() {
        static std::unique_ptr<std::vector<basic_log>> logs = std::make_unique<std::vector<basic_log>>();
        return *logs;
    }
    void Logs::Add(basic_log&& what) {
        GetLogs().emplace_back(std::move(what));
    }
    void Logs::Add(const level severity, std::string_view what) {
        GetLogs().emplace_back(basic_log{ severity, std::string(what) });
    }
    void Logs::Clear() {
        GetLogs().clear();
    }
    void Logs::ClearType(const level severity) {
        auto& logs = Logs::GetLogs();
        logs.erase(std::remove_if(logs.begin(), logs.end(), [severity](const basic_log& log) {
            return log.severity == severity;
            }), logs.end());
    }
    std::string_view Logs::GetLastLog() {
        auto& logs = Logs::GetLogs();
        return logs.empty() ? noLogs : logs.back().info;
    }
    std::string_view Logs::GetLastLog(const level severity) {
        auto& logs = Logs::GetLogs();
        std::string_view view = noLogs;

        for (auto log = logs.rbegin(); log != logs.rend(); ++log) {
            if (log->severity == severity) {
                view = log->info;
                break;
            }
        }
        return view;
    }
}