#ifndef W_TEXTBOX_H
#define W_TEXTBOX_H

#include "W_Common.h"
#include <string.h>
#include "W_Font.h"
#include "W_TextTable.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_TextureManager.h"
#include "W_HudElement.h"

namespace wolf
{
	constexpr float AL_Left = 0.0f;
	constexpr float AL_Center = 0.5f;
	constexpr float AL_Right = 1.0f;
	
	class TextBox: public HudElement
	{
	public:
		//Public Methods
		TextBox(Font* pFont, TextTable* localization, const bool& isSubpixel = false);
		~TextBox();
		void SetSize(const float& size);
		void SetString(const std::string& id);
		void SetStringRaw(const std::string& text);
		void SetTextColor(const glm::vec4& color);
		void SetTextAlignment(const float& alignment);
		void SetSubpixelBG(const glm::vec3& bgcolor);
		void Update(const float& p_fDelta);
		void Render(const glm::mat4& proj);	
	private:
		//Private Methods
		void UpdateString(const std::string& text);
		std::string ReplaceTextVars(const std::string& text);

		//Private Variables
		Font* m_font;
		TextTable* m_localization;
		bool m_hasVars = false;
		float m_alignmentFactor;
		float m_fontSize = 16.0f;
		std::string m_str;
		std::string m_prevText;
		glm::vec4 m_textcolor;
		glm::vec3 m_bgColor;
		std::vector<std::vector<Vertex>> m_glyphs;
		std::vector<wolf::VertexBuffer*> g_pVB;
		std::vector<wolf::VertexDeclaration*> g_pDecl;
		std::vector<wolf::Program*> g_pProgram;
		std::vector<wolf::Texture*> g_pTexture;
	};
}

#endif