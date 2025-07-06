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
		SteamConnector(const std::string_view& apiKey, const std::string_view& userId) :apiKey(apiKey), userId(userId) {}

		void setSteamAPIKey(const std::string_view& str) {
			apiKey = str;
		}
		void setSteamUserID(const std::string_view& str) {
			userId = str;
		}
		const std::string_view& getAPIKey()const {
			return apiKey;
		}
		const std::string_view& getUserID()const {
			return userId;
		}

		std::string get_API_URL(const SAPI_FUNCTION type) {
			std::string url = getTemplateUrl(type);

			if (!insertAPIKey(url)) {
				url.append(" - could not find < api > insertion position");
			}

			if (!insertUserID(url)) {
				url.append(" - could not find < user id >  insertion position");
			}

			return url;
		}

		std::string get_API_URL(const SAPI_FUNCTION type, const std::string_view& gameId) {
			std::string url = getTemplateUrl(type);

			if (!insertAPIKey(url)) {
				url.append(" - could not find < api > insertion position");
			}

			if (!insertUserID(url)) {
				url.append(" - could not find < user id >  insertion position");
			}

			if (!insertGameId(url, gameId)) {
				url.append(" - could not find < gameid/appid >  insertion position");
			}

			return url;
		}


	private:
		std::string getTemplateUrl(const SAPI_FUNCTION type) {
			switch (type) {
			case SAPI_FUNCTION::SUMMARY:			 return std::string(urlT_summary);
			case SAPI_FUNCTION::GAMES:				 return std::string(urlT_games);
			case SAPI_FUNCTION::RECENTLY_PLAYED:	 return std::string(urlT_recently_played);
			case SAPI_FUNCTION::ACHIEVEMENTS_PLAYER: return std::string(urlT_achievement_player);
			case SAPI_FUNCTION::ACHIEVEMENTS_GLOBAL: return std::string(urlT_achievement_global);
			case SAPI_FUNCTION::ACHIEVEMENTS_SCHEMA: return std::string(urlT_achievement_schema);
			default:
				break;
			}
			return "error template url - incorrect or not updated type";
		}
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
		bool insertGameId(std::string& templateUrl, const std::string_view& gameId)const {
			size_t pos;

			if (!findInsertionPoint(templateUrl, "appid", pos)) {
				if (!findInsertionPoint(templateUrl, "gameid", pos)) {
					return false;
				}
			}
			templateUrl.insert(pos, gameId);

			return true;
		}

		bool findInsertionPoint(const std::string_view& templateUrl, const std::string_view& lookFor, size_t& pos)const {
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