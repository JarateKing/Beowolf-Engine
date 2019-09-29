#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ComponentLookat.h"
#include "ComponentFollow.h"
#include "ComponentCameraPos.h"
#include "ComponentLifespan.h"
#include "ComponentBobbing.h"
#include "ComponentRotating.h"
#include "ComponentSphereCollider.h"
#include "ComponentCoinTimer.h"
#include "ComponentDestroyOnCollision.h"
#include "ComponentScore.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "W_Time.h"
#include "W_Model.h"

Common::SceneCamera* m_pSceneCamera;
Common::GameObjectManager* m_pGameObjectManager;

using namespace week2;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	// Initialize our Scene Manager
	Common::SceneManager::CreateInstance();

	// Initialize our Scene Camera and attach it to the Scene Manager
	m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Set up the component factory methods
	m_pGameObjectManager->RegisterComponentFactory("GOC_Bobbing", ComponentBobbing::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_Rotating", ComponentRotating::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_Lifespan", ComponentLifespan::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_RenderableMesh", ComponentRenderableMesh::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_CoinTimer", ComponentCoinTimer::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_CharacterController", ComponentCharacterController::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_SphereCollider", ComponentSphereCollider::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_DestroyOnCollision", ComponentDestroyOnCollision::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_Score", ComponentScore::CreateComponent);

	// Create an empty GameObject
	Common::GameObject* pCharacter = m_pGameObjectManager->CreateGameObject("../resources/objects/character.xml");
	pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	// COIN TIMER
	Common::GameObject* pCoinTimer = m_pGameObjectManager->CreateGameObject("../resources/objects/coin_timer.xml");

	// Do the same but with a lamp post
	Common::GameObject* pLampPost = m_pGameObjectManager->CreateGameObject("../resources/objects/lamp.xml");
	pLampPost->GetTransform().Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	pLampPost->GetTransform().Translate(glm::vec3(-10.0f, 0, -10.0f));

	// Do the same but with the ground
	Common::GameObject* pGround = m_pGameObjectManager->CreateGameObject("../resources/objects/ground.xml");
	pGround->GetTransform().Scale(glm::vec3(50.0f, 0.001f, 50.0f));

	// Camera that follows the player
	Common::GameObject* pThirdPersonCamera = m_pGameObjectManager->CreateGameObject();
	ComponentCameraPos* pThirdPersonCameraPos = new ComponentCameraPos();
	pThirdPersonCameraPos->SetCamera(m_pSceneCamera);
	pThirdPersonCamera->AddComponent(pThirdPersonCameraPos);
	ComponentFollow* pFollowCharacter = new ComponentFollow();
	pFollowCharacter->Follow(pCharacter);
	pFollowCharacter->SetOffset(glm::vec3(0.0f, 8.0f, -15.0f));
	pThirdPersonCamera->AddComponent(pFollowCharacter);
	ComponentLookat* pFollowCharacterRot = new ComponentLookat();
	pFollowCharacterRot->Follow(pCharacter);
	pFollowCharacterRot->SetOffset(glm::vec3(0.0f, 7.0f, 0.0f));
	pThirdPersonCamera->AddComponent(pFollowCharacterRot);

	// Camera that's above the lamp
	Common::GameObject* pStaticCamera = m_pGameObjectManager->CreateGameObject();
	pStaticCamera->GetTransform().Translate(glm::vec3(-10.0f, 25.0f, -10.0f));
	ComponentLookat* pLookatCharacter = new ComponentLookat();
	pLookatCharacter->Follow(pCharacter);
	pLookatCharacter->SetOffset(glm::vec3(0.0f, 7.0f, 0.0f));
	pStaticCamera->AddComponent(pLookatCharacter);
	ComponentCameraPos* pStaticCameraPos = new ComponentCameraPos();
	pStaticCameraPos->SetCamera(m_pSceneCamera);
	pStaticCameraPos->SetEnabled(false);
	pStaticCamera->AddComponent(pStaticCameraPos);
}

void BaseScene::Update()
{
	m_pGameObjectManager->Update(wolf::Time::Instance().deltaTime());
}

void BaseScene::Render()
{
	m_pGameObjectManager->SyncTransforms();
	Common::SceneManager::Instance()->Render();
}


