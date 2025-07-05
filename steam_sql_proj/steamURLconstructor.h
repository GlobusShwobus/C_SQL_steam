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

	class SteamConnector {
		
		std::string apiKey;
		std::string userId;

	public:
		SteamConnector() = default;
		SteamConnector(const std::string& apiKey, const std::string& userId) :apiKey(apiKey), userId(userId) {}

		void setSteamAPIKey(const std::string& str) {
			apiKey = str;
		}
		void setSteamUserID(const std::string& str) {
			userId = str;
		}
		const std::string& getAPIKey() {
			return apiKey;
		}
		const std::string& getUserID() {
			return userId;
		}


	private:

		bool insertAPIKey(std::string& templateUrl)const {
			size_t pos;
			if (!findInsertionPoint(templateUrl, "key", pos))
				return false;
			
			templateUrl.insert(pos, apiKey);

			return true;
		}
		bool insertUserID(std::string& templateUrl)const {
			size_t pos;
			if (!findInsertionPoint(templateUrl, "steamid", pos))
				return false;

			templateUrl.insert(pos, userId);

			return true;
		}
		bool insertGameId(std::string& templateUrl, const std::string& gameId)const {
			size_t pos;

			if (!findInsertionPoint(templateUrl, "appid", pos)) {
				if (!findInsertionPoint(templateUrl, "gameid", pos)) {
					return false;
				}
			}
			templateUrl.insert(pos, gameId);

			return true;
		}

		bool findInsertionPoint(std::string_view templateUrl, std::string_view lookFor, size_t& pos)const {
			pos = templateUrl.find(lookFor);
			if (pos == std::string::npos)
				return false;
			pos = templateUrl.find_first_of('=', pos) + 1;

			return true;
		}


		static constexpr std::string_view urlT_achievement_schema = "https://api.steampowered.com/ISteamUserStats/GetSchemaForGame/v0001/?appid=&name=1&description=1&key=&steamid=&format=json";
		static constexpr std::string_view urlT_achievement_global = "https://api.steampowered.com/ISteamUserStats/GetGlobalAchievementPercentagesForApp/v0002/?gameid=&format=json";
		static constexpr std::string_view urlT_achievement_player = "https://api.steampowered.com/ISteamUserStats/GetPlayerAchievements/v0001/?appid=&key=&steamid=&name=1&format=json";
		static constexpr std::string_view urlT_summary            = "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=&steamids=&format=json";
		static constexpr std::string_view urlT_games              = "https://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=&steamid=&include_appinfo=true&include_played_free_games=true&format=json";
		static constexpr std::string_view urlT_recently_played    = "https://api.steampowered.com/IPlayerService/GetRecentlyPlayedGames/v0001/?key=&steamid=&format=json";
	};





	std::string SAPItoString(const SAPI_FUNCTION t) {
		std::string s;
		
		switch (t) {
		case SAPI_FUNCTION::SUMMARY:			 s = "player summary";			   break;
		case SAPI_FUNCTION::GAMES:				 s = "player owned games";		   break;
		case SAPI_FUNCTION::RECENTLY_PLAYED:	 s = "player recent activity";	   break;
		case SAPI_FUNCTION::ACHIEVEMENTS_PLAYER: s = "player achievements for id"; break;
		case SAPI_FUNCTION::ACHIEVEMENTS_GLOBAL: s = "global achievements for id"; break;
		case SAPI_FUNCTION::ACHIEVEMENTS_SCHEMA: s = "id achievement schema";	   break;
		default:
			s = "BIG WRONG WII UUU WII UU";
			break;
		}
		return s;
	}

}