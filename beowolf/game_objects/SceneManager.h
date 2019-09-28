//------------------------------------------------------------------------
// SceneManager
//
// Created:	2012/12/23
// Author:	Carel Boers
//	
// A simple scene manager to manage our models and camera.
//------------------------------------------------------------------------

#ifndef COMMON_SCENEMANAGER_H
#define COMMON_SCENEMANAGER_H

#include "W_Model.h"
#include "W_Sprite.h"
#include "SceneCamera.h"

namespace Common
{
	class SceneManager
	{
		// Typedef for convenience
		typedef std::vector<wolf::Model*> ModelList;
		typedef std::vector<wolf::Sprite*> SpriteList;
	
		// A directional light
		struct DirectionalLight
		{
			glm::vec3		m_vDirection;
			wolf::Color4    m_ambient;
			wolf::Color4	m_diffuse;
			wolf::Color4	m_specular;
			DirectionalLight() : m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}
		};

		// A point light
		struct PointLight
		{
			glm::vec3		m_pos;
			float			m_radius;
			wolf::Color4	m_ambient;
			wolf::Color4	m_diffuse;
			wolf::Color4	m_specular;
			PointLight() : m_radius(0), m_pos(0,0,0), m_ambient(0,0,0,0), m_diffuse(0,0,0,0), m_specular(0,0,0,0) {}
		};

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		static void CreateInstance();
		static void DestroyInstance();
		static SceneManager* Instance();

		void AddModel(wolf::Model* p_pModel);
		void RemoveModel(wolf::Model* p_pModel);
		void Clear();

		void AddSprite(wolf::Sprite* p_pSprite);
		void RemoveSprite(wolf::Sprite* p_pSprite);
		void ClearSprites();

		void AttachCamera(SceneCamera* p_pCamera);
		SceneCamera* GetCamera();

		DirectionalLight* GetDefaultLight() { return m_pLight; }

		void Render();

		void SetPointLightPos(glm::vec3 pos);

	private:
		//------------------------------------------------------------------------------
		// Private methods.
		//------------------------------------------------------------------------------
		
		// Constructor/Destructor are private because we're a Singleton
		SceneManager();
		virtual ~SceneManager();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Static singleton instance
		static SceneManager* s_pSceneManagerInstance;

		// A list of models to render
		ModelList m_lModelList;

		// A list of sprites to render
		SpriteList m_lSpriteList;

		// A camera to view the scene
		SceneCamera* m_pCamera;
		
		// Default directional light
		DirectionalLight* m_pLight;

		// Character point light
		PointLight* m_pPlayerLight;

		// Lamp point light
		PointLight* m_pLampLight;
	};

} // namespace common

#endif // COMMON_SCENEMANAGER_H