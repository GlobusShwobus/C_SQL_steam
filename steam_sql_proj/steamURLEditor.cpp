#include "steamURLEditor.h"

namespace ORDO {
	void SteamURLEditor::setSteamAPIKey(std::string_view str) {
		apiKey = str;
	}
	void SteamURLEditor::setSteamUserID(std::string_view str) {
		userId = str;
	}
	std::string_view SteamURLEditor::getAPIKey()const {
		return apiKey;
	}
	std::string_view SteamURLEditor::getUserID()const {
		return userId;
	}

	std::string SteamURLEditor::get_API_URL(const DATA_TYPE type) {
		std::string url = getTemplateUrl(type);

		if (!insertAPIKey(url)) {
			url.append(" - could not find < api > insertion position");
		}

		if (!insertUserID(url)) {
			url.append(" - could not find < user id >  insertion position");
		}

		return url;
	}

	std::string SteamURLEditor::get_API_URL(const DATA_TYPE type, std::string_view gameId) {
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

	std::string SteamURLEditor::getTemplateUrl(const DATA_TYPE type)const {
		switch (type) {
		case DATA_TYPE::SUMMARY:			 return std::string(urlT_summary);
		case DATA_TYPE::GAMES:				 return std::string(urlT_games);
		case DATA_TYPE::RECENTLY_PLAYED:	 return std::string(urlT_recently_played);
		case DATA_TYPE::ACHIEVEMENTS_PLAYER: return std::string(urlT_achievement_player);
		case DATA_TYPE::ACHIEVEMENTS_GLOBAL: return std::string(urlT_achievement_global);
		case DATA_TYPE::ACHIEVEMENTS_SCHEMA: return std::string(urlT_achievement_schema);
		default:
			break;
		}
		return "error template url - incorrect or not updated type";
	}
	bool SteamURLEditor::insertAPIKey(std::string& templateUrl)const {
		size_t pos;
		if (!findInsertionPoint(templateUrl, "key", pos))
			return false;

		templateUrl.insert(pos, apiKey);

		return true;
	}
	bool SteamURLEditor::insertUserID(std::string& templateUrl)const {
		size_t pos;
		if (!findInsertionPoint(templateUrl, "steamid", pos))
			return false;

		templateUrl.insert(pos, userId);

		return true;
	}
	bool SteamURLEditor::insertGameId(std::string& templateUrl, std::string_view gameId)const {
		size_t pos;

		if (!findInsertionPoint(templateUrl, "appid", pos)) {
			if (!findInsertionPoint(templateUrl, "gameid", pos)) {
				return false;
			}
		}
		templateUrl.insert(pos, gameId);

		return true;
	}

	bool SteamURLEditor::findInsertionPoint(std::string_view templateUrl, std::string_view lookFor, size_t& pos)const {
		pos = templateUrl.find(lookFor);
		if (pos == std::string::npos)
			return false;
		pos = templateUrl.find_first_of('=', pos) + 1;

		return true;
	}


	std::string SAPItoString(const DATA_TYPE t) {
		std::string s;

		switch (t) {
		case DATA_TYPE::SUMMARY:			 s = "player summary";			   break;
		case DATA_TYPE::GAMES:				 s = "player owned games";		   break;
		case DATA_TYPE::RECENTLY_PLAYED:	 s = "player recent activity";	   break;
		case DATA_TYPE::ACHIEVEMENTS_PLAYER: s = "player achievements for id"; break;
		case DATA_TYPE::ACHIEVEMENTS_GLOBAL: s = "global achievements for id"; break;
		case DATA_TYPE::ACHIEVEMENTS_SCHEMA: s = "id achievement schema";	   break;
		default:
			s = "BIG WRONG WII UUU WII UU";
			break;
		}
		return s;
	}
}

