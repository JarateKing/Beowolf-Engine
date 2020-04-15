#ifndef FONT_H
#define FONT_H

#include "W_Common.h"
#include "W_TextureManager.h"
#include <map>
#include <vector>

namespace wolf
{
	struct glyph {
		int texnum;
		float x1, y1, x2, y2;
		float xoffset, yoffset;
		float xadvance;
	};
	
	class Font
	{
	public:
		Font(const std::string& dir, const std::string& file);
		~Font();
		wolf::Texture* GetTexture(const int& number);
		int GetTextureNum(const char& input);
		int GetTotalTextures();
		float GetCharX1(const char& input);
		float GetCharX2(const char& input);
		float GetCharY1(const char& input);
		float GetCharY2(const char& input);
		float GetCharXOffset(const char& input);
		float GetCharYOffset(const char& input);
		float GetCharXAdvance(const char& input);
		float GetCharWidth(const char& input);
		float GetCharHeight(const char& input);
		float GetLineOffset();
	
	private:
		std::string m_dirPrefix;
		std::string m_dirFntName;
		std::map<char, glyph> m_characters;
		std::vector<std::string> m_textures;
		int m_texCount;
		int m_srcWidth;
		int m_srcHeight;
		int m_lineHeight;
		int m_baseHeight;
	};
}

#endif