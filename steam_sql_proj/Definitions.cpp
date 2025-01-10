#include "Entity.h"
#include "MakeFile.h"

namespace ORDO {
    bool DATA::Empty()const {
        return data.empty();
    }
    void DATA::Clear() {
        data.clear();
    }
    bool DATA::Add(std::unique_ptr<nlohmann::json> _data) {
        if (_data == nullptr) { LogError("Recieved nullptr"); return false; }
        if (_data->empty()) { return false; }

        data.push_back(std::move(_data));
        return true;
    }
    void DATA::Display()const {
        if (!data.empty()) {
            for (const std::unique_ptr<nlohmann::json>& entry : data) {
                printf("\n%s", entry->dump().c_str());
            }
            TWait();
        }
        else {
            LogMessage("No data to display...");
        }
    }
    const std::vector<std::unique_ptr<nlohmann::json>>& DATA::Read()const {
        return data;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ACHIEVEMENTS::Add(std::unique_ptr<nlohmann::json> for_global, std::unique_ptr<nlohmann::json> for_player, std::unique_ptr<nlohmann::json> for_schema) {
        if (for_global == nullptr || for_player == nullptr || for_schema == nullptr) {
            LogError("ACHIEVEMENTS::Add: Recieved nullptr");
            return false;
        }
        if (for_global->empty() || for_player->empty() || for_schema->empty()) {
            return false;
        }

        global.Add(std::move(for_global));
        player.Add(std::move(for_player));
        schema.Add(std::move(for_schema));
        return true;
    }
    void ACHIEVEMENTS::Clear() {
        global.Clear();
        player.Clear();
        schema.Clear();
    }
    void ACHIEVEMENTS::Display(const ACHIEVEMENT type)const {
        if (type == ACHIEVEMENT::ach_global) { global.Display(); }
        else if (type == ACHIEVEMENT::ach_player) { player.Display(); }
        else if (type == ACHIEVEMENT::ach_schema) { schema.Display(); }
        else {
            LogError("ACHIEVEMENTS::Display invalid ENTITY type entered");
        }
    }

    std::unique_ptr<std::vector<std::string>> ACHIEVEMENTS::GetStoredNames()const {
        std::unique_ptr<std::vector<std::string>> names = std::make_unique<std::vector<std::string>>();

        for (const std::unique_ptr<nlohmann::json>& json : player.data) {
            if (json != nullptr && json->contains("playerstats") && (*json)["playerstats"].is_object() && (*json)["playerstats"].contains("gameName")) {
                names->push_back((*json)["playerstats"]["gameName"]);
            }
        }

        return names;
    }
    const std::vector<std::unique_ptr<nlohmann::json>>& ACHIEVEMENTS::Read(const ACHIEVEMENT type)const {

        if (type == ACHIEVEMENT::ach_global) { return global.data; }
        else if (type == ACHIEVEMENT::ach_player) { return player.data; }
        else if (type == ACHIEVEMENT::ach_schema) { return schema.data; }
        else {
            throw std::invalid_argument("ACHIEVEMENT::MakeFiles somehow invalid type");
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    DATA& MDATA::PlayerSummary() {
        return player_summary;
    }
    DATA& MDATA::PlayerRecent() {
        return player_recent;
    }
    DATA& MDATA::PlayerGames() {
        return player_games;
    }
    ACHIEVEMENTS& MDATA::Achievements() {
        return achievements;
    }

    void MDATA::Clear() {
        player_summary.data.clear();
        player_recent.data.clear();
        player_games.data.clear();
        achievements.global.data.clear();
        achievements.player.data.clear();
        achievements.schema.data.clear();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    OUTPUT_HANDLE::OUTPUT_HANDLE(const std::filesystem::path& path, const nlohmann::json& data) {
        try {
            output.open(path, std::ios::out);
            output << data.dump(4);
        }
        catch (const std::exception& e) {
            LogError(std::string("OUTPUT_HANDLE:: ") + e.what());
        }
    }
    OUTPUT_HANDLE::OUTPUT_HANDLE(const std::filesystem::path& path, const std::vector<std::string>& data) {
        try {
            output.open(path, std::ios::out);
            for (const auto& each : data) {
                output << each << "\n";
            }
        }
        catch (const std::exception& e) {
            LogError(std::string("OUTPUT_HANDLE:: ") + e.what());
        }
    }
    INPUT_HANDLE::INPUT_HANDLE(const std::filesystem::path& file_path, nlohmann::json& into) {
        try {
            input.open(file_path, std::ios::in);
            input >> into;
        }
        catch (const std::exception& e) {
            LogError(std::string("INPUT_HANDLE:: ") + e.what());
        }
    }


    const std::vector<std::string> FILE_MANAGER::GetFolderContents()const {
        std::vector<std::string> file_names;
        try {
            for (const auto& entry : std::filesystem::directory_iterator(some_path)) {
                file_names.push_back(entry.path().stem().string());
            }
        }
        catch (const std::exception& e) {
            LogError(std::string("ERROR FILE_MANAGER::GetFolderContents ") + e.what());
            file_names.clear();
        }
        return file_names;
    }
    void FILE_MANAGER::AssignNameCopyCheck(const std::vector<std::string>& folder_contents, std::string& name, const std::string& extension, int copy_index)const {
        const auto it = std::find(folder_contents.cbegin(), folder_contents.cend(), name);

        if (it != folder_contents.cend()) {
            name = name.substr(0, name.find(extension));
            name += extension + std::to_string(copy_index);
            AssignNameCopyCheck(folder_contents, name, extension, copy_index + 1);
        }
    }
    void FILE_MANAGER::MakeJSON(const std::vector<std::unique_ptr<nlohmann::json>>& data)const {
        try {
            if (data.empty()) {
                LogMessage("FILE_MANAGER::MakeJSON empty container");
                return;
            }

            std::vector<std::string> folder_contents = GetFolderContents();
            TRefresh();

            std::string new_file_name = ISTR::InputStr("\nEnter file name >>> ");

            for (const auto& each : data) {
                AssignNameCopyCheck(folder_contents, new_file_name, "_copy");
                folder_contents.push_back(new_file_name);
                const std::filesystem::path new_file_path = some_path / (new_file_name + ".json");
                OUTPUT_HANDLE handle(new_file_path, *each);
                LogMessage(std::string("JSON file created for >> ") + new_file_name);
            }
        }
        catch (const std::exception& e) {
            LogError(e.what());
        }
    }
    void FILE_MANAGER::MakeManyJSON(const std::vector<std::unique_ptr<nlohmann::json>>& list, const std::unique_ptr<std::vector<std::string>>& names)const {
        try {
            if (names != nullptr && list.size() != names->size()) {
                throw std::invalid_argument("FILE_MANAGER::MakeManyJSON number of JSON objects and names do not match");
            }
            if (list.empty()) {
                LogMessage("FILE_MANAGER::MakeManyJSON empty container");
                return;
            }
            const std::vector<std::string> folder_contents = GetFolderContents();

            for (std::string& each : *names.get()) {
                AssignNameCopyCheck(folder_contents, each, "_copy");
            }

            size_t index = 0;

            for (const std::unique_ptr<nlohmann::json>& each : list) {
                const std::filesystem::path new_file_path = some_path / ((*names)[index] + ".json");
                OUTPUT_HANDLE handle(new_file_path, *each.get());

                LogMessage(std::string("JSON file created for >> ") + (*names)[index]);
                index++;
            }
        }
        catch (const std::exception& e) {
            LogError(e.what());
        }
    }
}

