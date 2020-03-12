#include <string>
#include <vector>

class LoadingScreen {
public:
	LoadingScreen();
	~LoadingScreen();
	void AddModel(std::string model);
	void Load();
private:
	std::vector<std::string> m_models;
};