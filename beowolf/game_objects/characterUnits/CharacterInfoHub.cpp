#include "CharacterInfoHub.h"
#include "JSON/json.hpp"
#include "W_ResourceLoader.h"
#include <sstream>
#include <fstream>
#include <iostream>

const std::vector<std::string> INFO_NAMES = { "HP", "MaxMovement", "MaxAttack", "MinAttack", "Health", "Damage", "Defense" };

CharacterInfoHub::CharacterInfoHub()
{

}

CharacterInfoHub::~CharacterInfoHub()
{

}

void CharacterInfoHub::AddCharacter(std::string p_characterJson, std::string p_characterName)
{
	InfoBit temp;
	temp.m_name = p_characterName;

	std::ifstream jsonIn(wolf::ResourceLoader::Instance().getJSONObject(p_characterJson));
	nlohmann::json jsonData;

	if (jsonIn) 
	{
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		jsonData = nlohmann::json::parse(jsonFileData);

		if (jsonData.contains("Character")) 
		{
			std::string tempDesc = jsonData["Character"]["BasicDescription"];
			temp.m_desc = tempDesc;
			temp.m_type = "Character";
			for (int i = 0; i < INFO_NAMES.size(); i++)
			{
				if (jsonData["Character"].contains(INFO_NAMES[i]))
				{
					temp.m_info[INFO_NAMES[i]] = jsonData["Character"][INFO_NAMES[i]];

					if (INFO_NAMES[i] == "HP")
						temp.m_info["Health"] = temp.m_info["HP"];
				}
			}
		}
	}
	m_infoBits.push_back(temp);
}

void CharacterInfoHub::AddEnemyType(std::string p_enemyJson, std::string p_enemyName)
{
	InfoBit temp;
	temp.m_name = p_enemyName;

	std::ifstream jsonIn(wolf::ResourceLoader::Instance().getJSONObject(p_enemyJson));
	nlohmann::json jsonData;

	if (jsonIn)
	{
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		jsonData = nlohmann::json::parse(jsonFileData);

		if (jsonData.contains("Enemy"))
		{
			std::string tempDesc = jsonData["Enemy"]["BasicDescription"];
			temp.m_desc = tempDesc;
			temp.m_type = "Enemy";
			for (int i = 0; i < INFO_NAMES.size(); i++)
			{
				if (jsonData["Enemy"].contains(INFO_NAMES[i]))
				{
					temp.m_info[INFO_NAMES[i]] = jsonData["Enemy"][INFO_NAMES[i]];

					if (INFO_NAMES[i] == "HP")
						temp.m_info["Health"] = temp.m_info["HP"];
				}
			}
		}
	}
	m_infoBits.push_back(temp);
}

void CharacterInfoHub::AddItemType(std::string p_itemJson)
{
	InfoBit temp;

	std::ifstream jsonIn(wolf::ResourceLoader::Instance().getJSONObject(p_itemJson));
	nlohmann::json jsonData;

	if (jsonIn)
	{
		std::stringstream jsonFileStream;
		jsonFileStream << jsonIn.rdbuf();
		std::string jsonFileData = jsonFileStream.str();
		jsonData = nlohmann::json::parse(jsonFileData);

		if (jsonData.contains("Name"))
		{
			std::string tempName = jsonData["Name"];
			std::string tempDesc = jsonData["BasicDescription"];
			temp.m_name = tempName;
			temp.m_desc = tempDesc;
			temp.m_type = "Item";
			for (int i = 0; i < INFO_NAMES.size(); i++)
			{
				if (jsonData["Stats"].contains(INFO_NAMES[i]))
				{
					temp.m_info[INFO_NAMES[i]] = jsonData["Stats"][INFO_NAMES[i]];
				}
			}
		}
	}
	m_infoBits.push_back(temp);
}

void CharacterInfoHub::DamageEnemy(std::string p_enemyName, std::string p_characterName)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_enemyName) == 0)
		{
			for (int j = 0; j < m_infoBits.size(); j++)
			{
				if (m_infoBits.at(j).m_name.compare(p_characterName) == 0)
				{
					m_infoBits.at(i).m_info["HP"] -= m_infoBits.at(j).m_info["MaxAttack"] * (100 / m_infoBits.at(i).m_info["Defense"]);

					if (m_infoBits.at(i).m_info["HP"] < 0)
						m_infoBits.at(i).m_info["HP"] = 0;
				}
			}
		}
	}
}

void CharacterInfoHub::DamageCharacter(std::string p_characterName, std::string p_enemyName)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_characterName) == 0)
		{
			for (int j = 0; j < m_infoBits.size(); j++)
			{
				if (m_infoBits.at(j).m_name.compare(p_enemyName) == 0)
				{
					m_infoBits.at(i).m_info["HP"] -= m_infoBits.at(j).m_info["MaxAttack"] * (100 / m_infoBits.at(i).m_info["Defense"]);

					if (m_infoBits.at(i).m_info["HP"] < 0)
						m_infoBits.at(i).m_info["HP"] = 0;
				}
			}
		}
	}
}

void CharacterInfoHub::GivePlayerItem(std::string p_characterName, std::string p_itemName)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_itemName) == 0)
		{
			for (int j = 0; j < m_infoBits.size(); j++)
			{
				if (m_infoBits.at(j).m_name.compare(p_characterName) == 0)
				{
					std::map<std::string, float>::iterator it = m_infoBits.at(i).m_info.begin();
					while (it != m_infoBits.at(i).m_info.end())
					{
						m_infoBits.at(j).m_info[it->first] += it->second;
						it++;
					}
				}
			}
		}
	}
}

float CharacterInfoHub::GetStat(std::string p_characterName, std::string p_statID)
{
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		if (m_infoBits.at(i).m_name.compare(p_characterName) == 0)
		{
			if (m_infoBits.at(i).m_info.count(p_statID))
				return m_infoBits.at(i).m_info[p_statID];
		}
	}
	return -1;
}

void CharacterInfoHub::PrintOutInfo()
{
	std::cout << "Test Info Character" << std::endl << "====================" << std::endl;
	for (int i = 0; i < m_infoBits.size(); i++)
	{
		std::cout << "Name: " << m_infoBits.at(i).m_name << std::endl;
		std::cout << "Desc: " << m_infoBits.at(i).m_desc << std::endl;
		std::cout << "Type: " << m_infoBits.at(i).m_type << std::endl;

		std::map<std::string, float>::iterator it = m_infoBits.at(i).m_info.begin();
		while (it != m_infoBits.at(i).m_info.end())
		{
			std::cout << it->first << " | ";
			std::cout << it->second << std::endl;
			it++;
		}
	}
	std::cout << "====================" << std::endl;
}