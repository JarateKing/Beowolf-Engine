#ifndef CHARACTERINFOHUB_H
#define CHARACTERINFOHUB_H

#include <string>
#include <map>
#include <vector>

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
		CharacterInfoHub();
		~CharacterInfoHub();
		
		void AddCharacter(std::string p_characterJson, std::string p_characterName);
		void AddEnemyType(std::string p_enemyJson, std::string p_enemyName);
		void AddItemType(std::string p_itemJson);

		void DamageEnemy(std::string p_enemyName, int p_damage);
		void DamageCharacter(std::string p_characterName, int p_damage);

		void GivePlayerItem(std::string p_characterName, std::string p_itemName);


	private:
		std::vector<InfoBit> m_infoBits;
		void PrintOutInfo();
};

#endif