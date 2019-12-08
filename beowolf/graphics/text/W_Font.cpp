#include "W_Font.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace wolf
{
	Font::Font(std::string dir, std::string file)
	{
		m_dirPrefix = dir;
		m_dirFntName = file;
	
		std::ifstream fileReader(dir + file);
	
		if (!fileReader.is_open())
			return;
	
		std::string line;
	
		// skip the first line
		std::getline(fileReader, line);
	
		// get the next line
		std::getline(fileReader, line, '=');
		std::getline(fileReader, line, '=');
		m_lineHeight = std::stoi(line);
		std::getline(fileReader, line, '=');
		m_baseHeight = std::stoi(line);
		std::getline(fileReader, line, '=');
		m_srcWidth = std::stoi(line);
		std::getline(fileReader, line, '=');
		m_srcHeight = std::stoi(line);
		std::getline(fileReader, line, '=');
		m_texCount = std::stoi(line);
		std::getline(fileReader, line);
	
		// skip the texture list
		for (int i = 0; i < m_texCount; i++)
		{
			std::getline(fileReader, line, '\"');
			std::getline(fileReader, line, '.');
			m_textures.push_back(dir + line + ".tga");
			std::getline(fileReader, line);
		}
	
		int charCount = 0;
		std::getline(fileReader, line, '=');
		std::getline(fileReader, line);
		charCount = std::stoi(line);
	
		for (int i = 0; i < charCount; i++)
		{
			std::getline(fileReader, line, '=');
			std::getline(fileReader, line, '=');
			int id = std::stoi(line);
			std::getline(fileReader, line, '=');
			int x = std::stoi(line);
			std::getline(fileReader, line, '=');
			int y = std::stoi(line);
			std::getline(fileReader, line, '=');
			int width = std::stoi(line);
			std::getline(fileReader, line, '=');
			int height = std::stoi(line);
			std::getline(fileReader, line, '=');
			int xoffset = std::stoi(line);
			std::getline(fileReader, line, '=');
			int yoffset = std::stoi(line);
			std::getline(fileReader, line, '=');
			int xadvance = std::stoi(line);
			std::getline(fileReader, line, '=');
			int texnum = std::stoi(line);
			std::getline(fileReader, line);
	
			m_characters[id] = {texnum, (float)x, (float)y, (float)width, (float)height, (float)xoffset, (float)yoffset, (float)xadvance};
		}
	}
	
	Font::~Font()
	{
		
	}
	
	wolf::Texture* Font::GetTexture(int number)
	{
		if (number >= m_textures.size())
			return NULL;
	
		return wolf::TextureManager::CreateTexture(m_textures[number]);
	}
	
	int Font::GetTextureNum(const char& input)
	{
		return m_characters[input].texnum;
	}
	
	int Font::GetTotalTextures()
	{
		return m_texCount;
	}
	
	float Font::GetCharX1(const char& input)
	{
		return m_characters[input].x1 / m_srcWidth;
	}
	
	float Font::GetCharX2(const char& input)
	{
		return (m_characters[input].x1 + m_characters[input].x2) / m_srcWidth;
	}
	
	float Font::GetCharY1(const char& input)
	{
		return 1.0f - m_characters[input].y1 / m_srcHeight;
	}
	
	float Font::GetCharY2(const char& input)
	{
		return 1.0f - (m_characters[input].y1 + m_characters[input].y2) / m_srcHeight;
	}
	
	float Font::GetCharXOffset(const char& input)
	{
		return m_characters[input].xoffset / m_srcWidth;
	}
	
	float Font::GetCharYOffset(const char& input)
	{
		return m_characters[input].yoffset / m_srcHeight;
	}
	
	float Font::GetCharXAdvance(const char& input)
	{
		return m_characters[input].xadvance / m_srcWidth;
	}
	
	float Font::GetCharWidth(const char& input)
	{
		return m_characters[input].x2 / m_srcWidth;
	}
	
	float Font::GetCharHeight(const char& input)
	{
		return m_characters[input].y2 / m_srcHeight;
	}
	
	float Font::GetLineOffset()
	{
		return ((float)m_lineHeight) / m_baseHeight;
	}
}