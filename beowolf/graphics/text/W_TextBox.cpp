#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GL/glfw.h>
#include "W_Common.h"
#include "W_VertexBuffer.h"
#include "W_TextBox.h"
#include "W_ResourceLoader.h"
#include "W_ProjectionMatrix.h"
#include <iostream>
#include <string>

namespace wolf
{
	constexpr float DESIRED_width = 1920.0f;
	constexpr float DESIRED_height = 1080.0f;
	constexpr float FONTSIZE = 1.0f / 56.0f;
	
	static const Vertex squareVertices[] = {
			{ 0.0f, 0.0f, 1.0f,	1, 1, 1, 1, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 1.0f,	1, 1, 1, 1, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f,	1, 1, 1, 1, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f,	1, 1, 1, 1, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,	1, 1, 1, 1, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,	1, 1, 1, 1, 0.0f, 0.0f },
	};
	
	TextBox::TextBox(Font* pFont, TextTable* localization, bool isSubpixel)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		m_textcolor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_font = pFont;
		m_localization = localization;
		m_alignmentFactor = AL_Left;
		m_bgColor = glm::vec3(0.5, 0.5, 0.5);
	
		for (int i = 0; i < m_font->GetTotalTextures(); i++)
		{
			auto shaderpair = wolf::ResourceLoader::Instance().getShaders((isSubpixel) ? "font_msdf_subpixel" : "font_msdf");
			g_pProgram.push_back(wolf::ProgramManager::CreateProgram(shaderpair));
			g_pVB.push_back(wolf::BufferManager::CreateVertexBuffer(squareVertices, sizeof(Vertex) * 6));
	
			g_pDecl.push_back(new wolf::VertexDeclaration());
			g_pDecl[i]->Begin();
			g_pDecl[i]->AppendAttribute(AT_Position, 3, CT_Float);
			g_pDecl[i]->AppendAttribute(AT_Color, 4, CT_UByte);
			g_pDecl[i]->AppendAttribute(AT_TexCoord1, 2, CT_Float);
			g_pDecl[i]->AppendAttribute(AT_Normal, 3, CT_Float);
			g_pDecl[i]->SetVertexBuffer(g_pVB[i]);
			g_pDecl[i]->End();
	
			g_pTexture.push_back(m_font->GetTexture(i));
		}
	}
	
	TextBox::~TextBox()
	{
	
	}

	void TextBox::SetSize(float size) {
		m_fontSize = size;
	}

	void TextBox::SetString(const std::string& id) {
		SetStringRaw(m_localization->GetString(id));
	}
	
	void TextBox::SetStringRaw(const std::string& text)
	{
		m_str = text;
		m_hasVars = text.find('$') != text.find_last_of('$');
		if (m_hasVars)
			UpdateString(ReplaceTextVars(text));
		else
			UpdateString(text);
	}

	void TextBox::UpdateString(const std::string& text)
	{
		m_prevText = text;
		m_glyphs.clear();
	
		float offsetHead = 0.0f;
		float offsetLine = 0.0f;
		float lineOffset = m_font->GetLineOffset();
	
		glm::vec2 screenDimensions = ProjMatrix::GetScreenSize();
		float aspectFactor = (screenDimensions.x / screenDimensions.y) / (16.0 / 9.0);
		float widthFactor = DESIRED_width / m_xBound;
		float heightFactor = DESIRED_height * aspectFactor / m_yBound;
	
		bool isNewWord = true;
		std::vector<std::vector<Vertex>>* lines = new std::vector<std::vector<Vertex>>[m_font->GetTotalTextures()];
		std::vector<Vertex>* currentLine = new std::vector<Vertex>[m_font->GetTotalTextures()];
		std::vector<float> lineSpaceRemaining;
		for (int i = 0; i < text.length(); i++)
		{
			// check if word won't fit on line
			if (offsetHead > 0 && isNewWord)
			{
				std::string remaining = text.substr(i);
				float postHead = offsetHead;
				int ptr = 0;
				
				for (int i = 0; i < remaining.length(); i++)
				{
					if (remaining.at(i) != ' ')
						postHead += m_font->GetCharXAdvance(remaining.at(i)) * (FONTSIZE * m_fontSize) * widthFactor;
					else
						break;
				}
	
				if (postHead >= 1 - m_font->GetCharXAdvance('W') * (FONTSIZE * m_fontSize) * widthFactor)
				{
					lineSpaceRemaining.push_back(1 - offsetHead);
					offsetHead = 0;
					offsetLine += lineOffset * (FONTSIZE * m_fontSize);
					for (int j = 0; j < m_font->GetTotalTextures(); j++)
					{
						lines[j].push_back(currentLine[j]);
						currentLine[j].clear();
					}
				}
			}
	
			char cur = text.at(i);
	
			int tex = m_font->GetTextureNum(cur);
	
			float x1 = offsetHead;
			float x2 = offsetHead + m_font->GetCharWidth(cur) * (FONTSIZE * m_fontSize) * widthFactor;
			float y1 = (offsetLine + (m_font->GetCharYOffset(cur))) * heightFactor * (FONTSIZE * m_fontSize);
			float y2 = (offsetLine + (m_font->GetCharYOffset(cur) + m_font->GetCharHeight(cur))) * heightFactor * (FONTSIZE * m_fontSize);
	
			float u1 = m_font->GetCharX1(cur);
			float u2 = m_font->GetCharX2(cur);
			float v1 = m_font->GetCharY1(cur);
			float v2 = m_font->GetCharY2(cur);
	
			currentLine[tex].push_back(Vertex({ x1, y1,	m_zPos, 1, 1, 1, 1, u1, v1}));
			currentLine[tex].push_back(Vertex({ x1, y2,	m_zPos, 1, 1, 1, 1, u1, v2}));
			currentLine[tex].push_back(Vertex({ x2, y2,	m_zPos, 1, 1, 1, 1, u2, v2}));
			currentLine[tex].push_back(Vertex({ x2, y2,	m_zPos, 1, 1, 1, 1, u2, v2}));
			currentLine[tex].push_back(Vertex({ x2, y1,	m_zPos, 1, 1, 1, 1, u2, v1}));
			currentLine[tex].push_back(Vertex({ x1, y1,	m_zPos, 1, 1, 1, 1, u1, v1}));
	
			offsetHead += m_font->GetCharXAdvance(cur) * (FONTSIZE * m_fontSize) * widthFactor;
			if (offsetHead >= 1 - m_font->GetCharXAdvance('W') * (FONTSIZE * m_fontSize) * widthFactor)
			{
				lineSpaceRemaining.push_back(1 - offsetHead);
				offsetHead = 0;
				offsetLine += lineOffset * (FONTSIZE * m_fontSize);
				for (int j = 0; j < m_font->GetTotalTextures(); j++)
				{
					lines[j].push_back(currentLine[j]);
					currentLine[j].clear();
				}
			}
	
			if (isNewWord)
				isNewWord = false;
			else
				isNewWord = cur == ' ';
		}
		for (int i = 0; i < m_font->GetTotalTextures(); i++)
		{
			lines[i].push_back(currentLine[i]);
		}
		lineSpaceRemaining.push_back(1 - offsetHead);
	
		// transfer buffers into proper vertex arrays
		for (int i = 0; i < m_font->GetTotalTextures(); i++)
		{
			m_glyphs.push_back(std::vector<Vertex>());
			for (int j = 0; j < lines[i].size(); j++)
			{
				for (int k = 0; k < lines[i][j].size(); k++)
				{
					m_glyphs[i].push_back(lines[i][j][k] + Vertex({ lineSpaceRemaining[j] * m_alignmentFactor, 0, 0, 0, 0, 0, 0, 0, 0 }));
				}
			}
		}
	
		// set up the parts for rendering
		for (int i = 0; i < m_font->GetTotalTextures(); i++)
		{
			if (m_glyphs[i].size() > 0)
			{
				if (g_pVB[i])
					wolf::BufferManager::DestroyBuffer(g_pVB[i]);
				if (g_pDecl[i])
					delete g_pDecl[i];
	
				g_pVB[i] = wolf::BufferManager::CreateVertexBuffer(&(m_glyphs[i][0]), sizeof(Vertex) * m_glyphs[i].size());
				g_pDecl[i] = new wolf::VertexDeclaration();
				g_pDecl[i]->Begin();
				Vertex::applyAttributes(g_pDecl[i]);
				g_pDecl[i]->SetVertexBuffer(g_pVB[i]);
				g_pDecl[i]->End();
			}
		}
	
		delete[] lines;
		delete[] currentLine;
	}
	
	void TextBox::SetTextColor(const glm::vec4& color)
	{
		m_textcolor = color;
	}
	
	void TextBox::SetTextAlignment(const float& alignment)
	{
		m_alignmentFactor = alignment;
	}

	void TextBox::SetSubpixelBG(const glm::vec3& bgcolor)
	{
		m_bgColor = bgcolor;
	}

	std::string TextBox::ReplaceTextVars(const std::string& text) {
		std::string toret = text;
		int pos = 0;
		while (pos != std::string::npos) {
			pos = toret.find('$', pos);
			if (pos != std::string::npos) {
				int next = toret.find('$', pos + 1);
				if (next != std::string::npos) {
					std::string replace = m_localization->GetVar(toret.substr(pos + 1, next - pos - 1));
					toret = toret.substr(0, pos) + replace + toret.substr(next + 1);
					pos = next + (replace.length() - (next - pos));
				}
			}
		}
		return toret;
	}

	void TextBox::Update(float p_fDelta) {
		if (m_hasVars) {
			std::string text = ReplaceTextVars(m_str);
			if (text != m_prevText)
				UpdateString(text);
		}
	}
	
	void TextBox::Render(glm::mat4 proj)
	{
		if (m_isVisible) {
			for (int i = 0; i < m_font->GetTotalTextures(); i++)
			{
				// Use shader program.
				g_pProgram[i]->Bind();
				g_pTexture[i]->Bind();

				// Bind Uniforms
				g_pProgram[i]->SetUniform("projection", proj);
				g_pProgram[i]->SetUniform("view", glm::mat4());
				g_pProgram[i]->SetUniform("world", m_world);
				g_pProgram[i]->SetUniform("color", m_textcolor);
				g_pProgram[i]->SetUniform("fontsize", m_fontSize);
				g_pProgram[i]->SetUniform("bgcolor", m_bgColor);
				g_pProgram[i]->SetUniform("tex", 0);

				// Set up source data
				g_pDecl[i]->Bind();

				// Draw!
				glDrawArrays(GL_TRIANGLES, 0, m_glyphs[i].size());
			}
		}
	}
}
