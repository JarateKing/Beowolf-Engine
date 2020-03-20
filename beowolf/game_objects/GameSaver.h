#include "W_Hud.h"

class GameSaver
{
public:
	GameSaver(wolf::Hud* hud);
	~GameSaver();
	void Update(float delta);
private:
	wolf::Hud* m_hud;
	float m_indicatorTime = 1.0f;
};