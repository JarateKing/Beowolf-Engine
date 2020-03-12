#include "LoadingScreen.h"
#include "W_ResourceLoader.h"
#include "BMWLoader.h"
#include <iostream>

LoadingScreen::LoadingScreen() {

}

LoadingScreen::~LoadingScreen() {

}

void LoadingScreen::AddModel(std::string model) {
	m_models.push_back(model);
}

void LoadingScreen::Load() {
	for (int i = 0; i < m_models.size(); i++) {
		std::cout << "Loading " << m_models[i] << '\n';
		wolf::BMWLoader::getInstance().loadFile(wolf::ResourceLoader::Instance().getModel(m_models[i]));
	}
}