#include "logMessage.h"
#include <memory>
namespace ORDO {
	
    std::vector<std::string>& ERROR_LOG::GetLogs() {
        static std::unique_ptr<std::vector<std::string>> error_log = std::make_unique<std::vector<std::string>>();
        return *error_log;
    }
    const std::string& ERROR_LOG::GetLastLog() {
        auto& log = ERROR_LOG::GetLogs();
        if (!log.empty()) {
            return log.back();
        }
        else {
            return "No errors";
        }
    }
    void ERROR_LOG::PopBack() {
        ERROR_LOG::GetLogs().pop_back();
    }
    void ERROR_LOG::Add(const std::string& what) {
        ERROR_LOG::GetLogs().emplace_back(what);
    }
    void ERROR_LOG::Add(const char* what) {
        ERROR_LOG::GetLogs().emplace_back(std::string(what));
    }
    void ERROR_LOG::Flush() {
        ERROR_LOG::GetLogs().clear();
    }

    static std::vector<std::string>& MESSAGE_LOG::GetLogs() {
        static std::unique_ptr<std::vector<std::string>> message_log = std::make_unique<std::vector<std::string>>();
        return *message_log;
    }
    static const std::string& MESSAGE_LOG::GetLastLog() {
        auto& log = MESSAGE_LOG::GetLogs();
        if (!log.empty()) {
            return log.back();
        }
        else {
            return "No messages";
        }
    }
    static void MESSAGE_LOG::PopBack() {
        MESSAGE_LOG::GetLogs().pop_back();
    }
    static void MESSAGE_LOG::Add(const std::string& what) {
        MESSAGE_LOG::GetLogs().emplace_back(what);
    }
    static void MESSAGE_LOG::Add(const char* what) {
        MESSAGE_LOG::GetLogs().emplace_back(std::string(what));
    }
    static void MESSAGE_LOG::Flush() {
        MESSAGE_LOG::GetLogs().clear();
    }
}