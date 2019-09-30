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
#include "EventManager.h"
#include "BulletPhysicsManager.h"
#include "ComponentRigidBody.h"
#include "ComponentGun.h"
#include "AIPathfinder.h"
#include "ComponentAIController.h"
#include "ComponentStunOnCollision.h"

Common::SceneCamera* m_pSceneCamera;
Common::GameObjectManager* m_pGameObjectManager;

using namespace week2;

BaseScene::BaseScene()
{
}

void BaseScene::Init()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Initialize our Scene Manager
	Common::SceneManager::CreateInstance();

	// Initialize our Scene Camera and attach it to the Scene Manager
	m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Physics Manager
	Common::BulletPhysicsManager::CreateInstance("../resources/objects/physics_materials.xml",
		"../resources/shaders/lines.vsh",
		"../resources/shaders/lines.fsh");

	// AI Manager
	week9::AIPathfinder::CreateInstance();
	week9::AIPathfinder::Instance()->Load("../resources/objects/AIPathfindingData.xml");

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
	m_pGameObjectManager->RegisterComponentFactory("GOC_Gun", ComponentGun::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_RigidBody", Common::ComponentRigidBody::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_StunOnCollision", ComponentStunOnCollision::CreateComponent);

	// Create an empty GameObject
	Common::GameObject* pCharacter = m_pGameObjectManager->CreateGameObject("../resources/objects/character.xml");
	pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	Common::ComponentRigidBody* pRigidBody = new Common::ComponentRigidBody();
	m_pGameObjectManager->SetGameObjectGUID(pCharacter, "PlayerCharacter");

	// COIN TIMER
	Common::GameObject* pCoinTimer = m_pGameObjectManager->CreateGameObject("../resources/objects/coin_timer.xml");

	// Do the same but with a lamp post
	Common::GameObject* pLampPost = m_pGameObjectManager->CreateGameObject("../resources/objects/lamp.xml");
	pLampPost->GetTransform().Scale(glm::vec3(0.5f, 0.5f, 0.5f));
	pLampPost->GetTransform().Translate(glm::vec3(-10.0f, 0, -10.0f));

	// Do the same but with the ground
	Common::GameObject* pGround = m_pGameObjectManager->CreateGameObject("../resources/objects/ground.xml");
	pGround->GetTransform().Scale(glm::vec3(50.0f, 0.001f, 50.0f));

	// 4 walls
	for (int i = 0; i < 4; i++)
	{
		Common::GameObject* pWall = m_pGameObjectManager->CreateGameObject("../resources/objects/wall.xml");
		pWall->GetTransform().Translate(glm::vec3(((i + 1) % 2) * (i / 2 * 2 - 1) * 50.0f, 50.0f, (i % 2) * (i / 2 * 2 - 1) * 50.0f));
		pWall->GetTransform().Rotate(glm::vec3((i % 2) * (i / 2 * 2 - 1) * 90.0f, 0.0f, ((i + 1) % 2) * (i / 2 * 2 - 1) * 90.0f));
		pWall->GetTransform().Scale(glm::vec3(50.0f, 0.001f, 50.0f));
	}


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			// add a crate
			Common::GameObject* pCrate = m_pGameObjectManager->CreateGameObject();
			pCrate->GetTransform().SetTranslation(glm::vec3(i * 10.0f - 5.0f, j * 2.0f + 3.0f, 0.0f));
			ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
			pRenderableComponent->Init("../resources/models/crate.pod", "../resources/models/", "../resources/shaders/textured.vsh", "../resources/shaders/textured.fsh");
			pCrate->AddComponent(pRenderableComponent);
			pRigidBody = new Common::ComponentRigidBody();
			pCrate->AddComponent(pRigidBody);
			pRigidBody->Init(new btBoxShape(btVector3(1.5f, 1.5f, 1.5f)), "Normal", 2.0f, glm::vec3(0.0f, -1.5f, 0.0f));
		}
	}

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

	// AI Game Object
	for (int i = 0; i < 3; i++)
	{
		Common::GameObject* pZombie = m_pGameObjectManager->CreateGameObject("../resources/objects/zombie.xml");
		pZombie->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));
		pZombie->GetTransform().SetTranslation(glm::vec3(i * 15.0f - 15.0f, 0.0f, 15.0f));
		m_pGameObjectManager->SetGameObjectGUID(pZombie, "Zombie");
		// Create a controller component for it
		week9::ComponentAIController* pAIControllerComponent = new week9::ComponentAIController();
		pAIControllerComponent->Init("../resources/objects/AIDecisionTree.xml");
		pZombie->AddComponent(pAIControllerComponent);
		// create physics, because this happens after set translation
		Common::ComponentRigidBody* pRigidBody = new Common::ComponentRigidBody();
		pZombie->AddComponent(pRigidBody);
		pRigidBody->Init(new btCapsuleShape(3.0f, 4.0f), "Player", 20.0f, glm::vec3(0.0f, -3.0f, 0.0f));
		pRigidBody->SetFlags(4, 8);
	}
}

void BaseScene::Update()
{
	double delta = wolf::Time::Instance().deltaTime();

	week9::AIPathfinder::Instance()->ClearBlockedNodes();
	Common::BulletPhysicsManager::Instance()->Update(delta);

	m_pGameObjectManager->Update(delta);
}

void BaseScene::Render()
{
	m_pGameObjectManager->SyncTransforms();
	Common::SceneManager::Instance()->Render();
}


