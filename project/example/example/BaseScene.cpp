#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include "BaseScene.h"
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ComponentLookat.h"
#include "ComponentFollow.h"
#include "ComponentCameraPos.h"
#include "ComponentMovingPointlight.h"
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

	// Create an empty GameObject
	Common::GameObject* pCharacter = m_pGameObjectManager->CreateGameObject();
	pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));

	// Create a renderable component for it
	ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("../resources/models/swat/Swat.pod", "../resources/models/swat/", "../resources/shaders/skinned.vsh", "../resources/shaders/skinned.fsh");

	pCharacter->AddComponent(pRenderableComponent);

	// Do the same but with a lamp post
	Common::GameObject* pLampPost = m_pGameObjectManager->CreateGameObject();
	pLampPost->GetTransform().Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	pLampPost->GetTransform().Translate(glm::vec3(-10.0f, 0, -10.0f));
	ComponentRenderableMesh* pLampRenderableComponent = new ComponentRenderableMesh();
	pLampRenderableComponent->Init("../resources/models/lamp.pod", "../resources/models/", "../resources/shaders/textured.vsh", "../resources/shaders/textured.fsh");
	pLampPost->AddComponent(pLampRenderableComponent);

	// Do the same but with the ground
	Common::GameObject* pGround = m_pGameObjectManager->CreateGameObject();
	pGround->GetTransform().Scale(glm::vec3(50.0f, 0.001f, 50.0f));
	ComponentRenderableMesh* pGroundRenderableComponent = new ComponentRenderableMesh();
	pGroundRenderableComponent->Init("../resources/models/plane.pod", "../resources/models/", "../resources/shaders/textured.vsh", "../resources/shaders/textured.fsh");
	pGround->AddComponent(pGroundRenderableComponent);

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

	// Light that follows the player
	Common::GameObject* pPlayerLight = m_pGameObjectManager->CreateGameObject();
	ComponentFollow* pLightFollowCharacter = new ComponentFollow();
	pLightFollowCharacter->Follow(pCharacter);
	pLightFollowCharacter->SetOffset(glm::vec3(0.0f, 7.0f, 0.0f));
	pPlayerLight->AddComponent(pLightFollowCharacter);
	ComponentMovingPointlight* pCharacterLightPositioner = new ComponentMovingPointlight();
	pPlayerLight->AddComponent(pCharacterLightPositioner);

	// Create an animation controller component for it
	ComponentAnimController* pAnimControllerComponent = new ComponentAnimController();
	pAnimControllerComponent->AddAnim("idle", "../resources/models/swat/anim/idle.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("walking", "../resources/models/swat/anim/walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("right_strafe", "../resources/models/swat/anim/right_strafe.pod", 0, 20, true);
	pAnimControllerComponent->AddAnim("right_strafe_walking", "../resources/models/swat/anim/right_strafe_walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("jump", "../resources/models/swat/anim/jump.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("left_strafe", "../resources/models/swat/anim/left_strafe.pod", 0, 20, true);
	pAnimControllerComponent->AddAnim("left_strafe_walking", "../resources/models/swat/anim/left_strafe_walking.pod", 0, 31, true);
	pAnimControllerComponent->AddAnim("left_turn_90", "../resources/models/swat/anim/left_turn_90.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("right_turn_90", "../resources/models/swat/anim/right_turn_90.pod", 0, 31, false);
	pAnimControllerComponent->AddAnim("standard_run", "../resources/models/swat/anim/standard_run.pod", 0, 22, true);
	pAnimControllerComponent->SetAnim("idle");
	pCharacter->AddComponent(pAnimControllerComponent);

	// Create a controller component for it
	ComponentCharacterController* pCharacterControllerComponent = new ComponentCharacterController();
	pCharacter->AddComponent(pCharacterControllerComponent);
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


