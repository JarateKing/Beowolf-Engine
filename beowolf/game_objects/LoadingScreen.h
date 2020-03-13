#include <string>
#include <vector>
#include "W_Hud.h"

class LoadingScreen {
public:
	LoadingScreen();
	~LoadingScreen();
	void AddModel(std::string model);
	void Load();
private:
	std::vector<std::string> m_models;
	wolf::Hud* m_hud;
	glm::mat4 m_proj;
};