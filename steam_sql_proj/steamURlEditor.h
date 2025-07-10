#pragma once
#include <string>
#include <string_view>

namespace ORDO {

	enum class SAPI_FUNCTIONS {
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

		// TODO: for get_API_URL edit extraInfo error message from a single place...
		std::string get_API_URL(const SAPI_FUNCTIONS type, std::string* extraInfo = nullptr);
		std::string get_API_URL(const SAPI_FUNCTIONS type, std::string_view gameId, std::string* extraInfo = nullptr);


	private:
		std::string getTemplateUrl(const SAPI_FUNCTIONS type)const;
		bool insertAPIKey(std::string& templateUrl)const;
		bool insertUserID(std::string& templateUrl)const;
		bool insertGameId(std::string& templateUrl, std::string_view gameId)const;

		bool findInsertionPoint(std::string_view templateUrl, std::string_view lookFor, size_t& pos)const;

		static constexpr std::string_view urlT_achievement_schema = "http://api.steampowered.com/ISteamUserStats/GetSchemaForGame/v2/?key=&appid=&l=en";//only if need some logo urls, otherwise worse than player in every way
		static constexpr std::string_view urlT_achievement_global = "https://api.steampowered.com/ISteamUserStats/GetGlobalAchievementPercentagesForApp/v0002/?gameid=&format=json";//only good for global % completion
		static constexpr std::string_view urlT_achievement_player = "http://api.steampowered.com/ISteamUserStats/GetPlayerAchievements/v1/?key=&steamid=&appid=&l=en";//this one is better than schema, use this
		static constexpr std::string_view urlT_summary            = "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=&steamids=&format=json";
		static constexpr std::string_view urlT_games              = "https://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=&steamid=&include_appinfo=true&include_played_free_games=true&format=json";//get game ids from here
		static constexpr std::string_view urlT_recently_played    = "https://api.steampowered.com/IPlayerService/GetRecentlyPlayedGames/v0001/?key=&steamid=&format=json";

		static constexpr std::string_view urlT_userSteamLevel     = "http://api.steampowered.com/IPlayerService/GetSteamLevel/v1/?key=&steamid=";//use this for steam level
		static constexpr std::string_view urlT_userSteamBadges    = "http://api.steampowered.com/IPlayerService/GetBadges/v1/?key=&steamid=";//also steam level, but no use for other data in current scope, not bad tho
	};




	//why do i need this????
	std::string SAPItoString(const SAPI_FUNCTIONS t);

}