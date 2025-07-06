#pragma once
#include <string>
#include <string_view>
namespace ORDO {
	enum class SAPI_FUNCTION {
		SUMMARY,
		GAMES,
		RECENTLY_PLAYED,
		ACHIEVEMENTS_PLAYER,
		ACHIEVEMENTS_GLOBAL,
		ACHIEVEMENTS_SCHEMA
	};

	class SteamURLEditor {
		
		std::string apiKey;
		std::string userId;

	public:
		SteamURLEditor() = default;
		SteamURLEditor(std::string_view apiKey, std::string_view userId) :apiKey(apiKey), userId(userId) {}

		void setSteamAPIKey(std::string_view str);
		void setSteamUserID(std::string_view str);
		std::string_view getAPIKey()const;
		std::string_view getUserID()const;

		std::string get_API_URL(const SAPI_FUNCTION type);
		std::string get_API_URL(const SAPI_FUNCTION type, std::string_view gameId);


	private:
		std::string getTemplateUrl(const SAPI_FUNCTION type)const;
		bool insertAPIKey(std::string& templateUrl)const;
		bool insertUserID(std::string& templateUrl)const;
		bool insertGameId(std::string& templateUrl, std::string_view gameId)const;

		bool findInsertionPoint(std::string_view templateUrl, std::string_view lookFor, size_t& pos)const;

		static constexpr std::string_view urlT_achievement_schema = "https://api.steampowered.com/ISteamUserStats/GetSchemaForGame/v0001/?appid=&name=1&description=1&key=&steamid=&format=json";
		static constexpr std::string_view urlT_achievement_global = "https://api.steampowered.com/ISteamUserStats/GetGlobalAchievementPercentagesForApp/v0002/?gameid=&format=json";
		static constexpr std::string_view urlT_achievement_player = "https://api.steampowered.com/ISteamUserStats/GetPlayerAchievements/v0001/?appid=&key=&steamid=&name=1&format=json";
		static constexpr std::string_view urlT_summary            = "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=&steamids=&format=json";
		static constexpr std::string_view urlT_games              = "https://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=&steamid=&include_appinfo=true&include_played_free_games=true&format=json";
		static constexpr std::string_view urlT_recently_played    = "https://api.steampowered.com/IPlayerService/GetRecentlyPlayedGames/v0001/?key=&steamid=&format=json";
	};




	//why do i need this????
	std::string SAPItoString(const SAPI_FUNCTION t);

}