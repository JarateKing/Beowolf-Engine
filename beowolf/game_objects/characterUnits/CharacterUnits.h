#ifndef CHARACTERUNITS_H
#define CHARACTERUNITS_H

#include <vector>
#include "BMWModel.h"
#include "ComponentHexPos.h"
#include "Healthbar.h"
#include "CooldownIndicator.h"
#include "sound/W_SoundEngine.h"
#include "Effect.h"

class CharacterUnits
{
	public:
		//Public Methods
		CharacterUnits(const std::string bmwFile, const std::string shaderFile, const int startTile, const std::string name, const HexGrid* grid, const float scale = 1.0f, const bool p_inverted = false, const glm::vec3 model_color = glm::vec3(1.0, 1.0, 1.0));
		~CharacterUnits();
		void Render( glm::mat4 view,  glm::mat4 proj,  glm::mat4 lightSpaceMatrix,  wolf::RenderFilterType type,  bool shadowPass,  unsigned int depthMapTexture);
		void Update(float deltaT);
		void SetCooldown(int val);
		void SetTile(int tile, bool updatePositionImmediately = false);
		void SetAnim(std::string animName);
		void SetSoundEngine(wolf::SoundEngine* soundEng);
		void SetHealthbarVisible(bool isVisible);
		void SetHealthbarPercent(float percent);
		void SetLighting(glm::vec4 ambLight, glm::vec4 difLight, glm::vec3 lightDir);
		void SetLightingDir(glm::vec3 dir);
		void setHasMoved(bool moved);
		void setSelected(bool selected);
		void InitDeath();
		void TakeDamage(std::string p_characterFrom, float mult = 1.0f, std::string particleEffectOverride = "", bool isParticleBillboarded = true);
		void Move(std::vector<int> p_path, float p_timeToComplete, bool p_attacking);
		void StartCooldown();
		void UpdateCooldown();
		void HealIndicator();
		const int GetTile() const;
		const int GetCooldown() const;
		bool InitDamage();
		bool isMoving() const;
		bool isAttacking() const;
		bool isDying() const;
		bool cmpf(float a, float b) const;
		const bool getHasMoved() const;
		const float GetDeathTimer() const;
		const float GetDamageReceivedMult() const;
		const std::vector<std::string> GetAttacker() const;
		const wolf::BMWModel* GetModel() const;
		const std::string GetName() const;
		const glm::vec3 GetPos() const;

	private:
		//Private Variables
		bool m_canTakeDamage = false;
		bool m_initiatingDamage = false;
		bool m_inverted = false;
		bool m_changed = false;
		bool m_dying = false;
		bool m_damaged = false;
		bool m_attacking = false;
		bool m_hasMoved = false;
		bool m_justMoved = false;
		bool m_isSelected = false;
		bool m_startedAttack = false;
		bool m_damageReceivingBillboarded = true;
		bool m_isHealthbarVisible = true;
		bool m_isCooldownVisible = false;
		int m_currTile, m_endTile, m_prevTile;
		int m_cooldownMax = 2;
		int m_cooldownCur = 0;
		int m_cooldownHeightAdjustment = 0.0f;
		float m_deathTimer = 0.0f;
		float m_timeAttacking = 0.0f;
		float m_timeDamaged = 0.0f;
		float m_damageReceivingMult = 1.0f;
		float m_scale = 0.0f;
		double m_deltaSum = 0.0;
		std::string m_name;
		std::string m_damageReceivingParticle = "";
		std::map<std::string, float> m_animTimes;
		std::vector<std::string> m_soundFiles;
		std::vector<std::string> m_characterAttacking;
		std::vector<Effect*> m_particleEffects;
		std::vector<Effect*> m_particleEffectsNoBillboard;
		glm::mat4 m_particleProjMatrix;
		glm::mat4 m_particleProjMatrixNoBillboard;
		week2::ComponentHexPos m_pos;
		wolf::BMWModel* m_model = NULL;
		wolf::SoundEngine* m_soundEngine = NULL;
		CooldownIndicator* m_cooldown = NULL;
		Healthbar* m_healthbar = NULL;
		const HexGrid* m_grid;
};

#endif