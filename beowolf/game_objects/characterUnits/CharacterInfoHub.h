#ifndef CHARACTERINFOHUB_H
#define CHARACTERINFOHUB_H

#include <string>
#include <map>
#include <vector>
#include <utility>

//Infobit struct used to store information
struct InfoBit
{
	std::map<std::string, float> m_info;
	std::string m_name = "";
	std::string m_type = "NULL";
	std::string m_desc = "";
};

class CharacterInfoHub
{
	public:
		//Public Methods
		CharacterInfoHub();
		~CharacterInfoHub();
		void AddCharacter(std::string p_characterJson, std::string p_characterName);
		void AddEnemyType(std::string p_enemyJson, std::string p_enemyName);
		void AddItemType(std::string p_itemJson);
		void DamageEnemy(std::string p_enemyName, std::string p_characterName, float mult = 1.0f);
		void DamageCharacter(std::string p_characterName, std::string p_enemyName, float mult = 1.0f);
		void GivePlayerItem(std::string p_characterName, std::string p_itemName);
		void UpdateStat(std::string p_characterName, std::string p_statID, float p_updatedValue);
		float GetStat(std::string p_characterName, std::string p_statID);
		std::vector<std::pair<std::string, float>> GetStats(std::string p_characterName);
		std::string GetDescription(std::string p_characterName);

	private:
		//Private Methods
		std::vector<InfoBit> m_infoBits;
		void PrintOutInfo();
};

#endif