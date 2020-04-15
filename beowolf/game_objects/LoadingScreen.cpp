#include "LoadingScreen.h"
#include "W_ResourceLoader.h"
#include "BMWLoader.h"
#include <iostream>

LoadingScreen::LoadingScreen() {
	m_hud = new wolf::Hud("resources/hud/load_screen.json");
	m_proj = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

LoadingScreen::~LoadingScreen() {

}

void LoadingScreen::AddModel(const std::string& model) {
	m_models.push_back(model);
}

void LoadingScreen::Load() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// set up view bounds
	int width, height;
	glfwGetWindowSize(&width, &height);
	height = height > 0 ? height : 1;
	glViewport(0, 0, width, height);

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup render options
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(false);

	for (int i = 0; i < m_models.size(); i++) {
		std::cout << "Loading " << m_models[i] << '\n';
		float percent = i / (m_models.size() + 1.0);
		
		// clear screen
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render hud with updated percent
		m_hud->SetVar("Loading_File", m_models[i]);
		m_hud->SetVar("Loading_Percent", std::to_string((int)(percent * 100.0)));
		m_hud->GetElement("loading_progressbar")->SetW(percent, true);
		m_hud->Update(1.0);
		m_hud->Render(m_proj);
		glfwSwapBuffers();

		wolf::BMWLoader::getInstance().loadFile(wolf::ResourceLoader::Instance().getModel(m_models[i]));
	}
}