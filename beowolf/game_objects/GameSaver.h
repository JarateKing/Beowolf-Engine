#include "W_Hud.h"
#include "characterUnits/CharacterInfoHub.h"
#include "characterUnits/CharacterManager.h"
#include "characterUnits/ScoreTracker.h"
#include "hexGrid/HexGrid.h"

class GameSaver
{
public:
	GameSaver(wolf::Hud* hud);
	~GameSaver();
	void Update(float delta);
	void SetInfo(CharacterInfoHub* hub, CharacterManager* manager, ScoreTracker* tracker, HexGrid* grid);

private:
	void SaveInfo();

	wolf::Hud* m_hud;
	float m_indicatorTime = 1.0f;
	CharacterInfoHub* m_hub;
	CharacterManager* m_manager;
	ScoreTracker* m_tracker;
	HexGrid* m_grid;
};