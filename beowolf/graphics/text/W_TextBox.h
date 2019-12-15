#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "W_Common.h"
#include <string.h>
#include "W_Font.h"
#include "W_TextTable.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_TextureManager.h"

namespace wolf
{
	constexpr float AL_Left = 0.0f;
	constexpr float AL_Center = 0.5f;
	constexpr float AL_Right = 1.0f;
	
	class TextBox
	{
	public:
		TextBox(Font* pFont, TextTable* localization);
		~TextBox();
		void SetBounds(const int& width, const int& height);
		void SetPos(const int& xpos, const int& ypos);
		void SetSize(float size);
		void SetString(const std::string& id);
		void SetStringRaw(const std::string& text);
		void SetTextColor(const glm::vec4& color);
		void SetTextAlignment(const float& alignment);
		void SetZ(const float& zpos);
		void Render(glm::mat4 proj);
	
	private:
		glm::vec4 m_textcolor;
		Font* m_font;
		TextTable* m_localization;
		float m_xBound;
		float m_yBound;
		float m_xPos;
		float m_yPos;
		std::vector<std::vector<Vertex>> m_glyphs;
		float m_alignmentFactor;
		glm::mat4 m_world;
		float m_fontSize = 16.0f;
		float m_zPos = 1.0f;
	
		std::vector<wolf::VertexBuffer*> g_pVB;
		std::vector<wolf::VertexDeclaration*> g_pDecl;
		std::vector<wolf::Program*> g_pProgram;
		std::vector<wolf::Texture*> g_pTexture;
	};
}

#endif