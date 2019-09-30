//------------------------------------------------------------------------
// ComponentAIController
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class implements an AI controller. It loads a decision tree from
// XML and processes the game state to decide it's best state.
//------------------------------------------------------------------------

#include "W_Model.h"
#include "ComponentAnimController.h"
#include "ComponentAIController.h"
#include "ComponentRenderableMesh.h"
#include "GameObject.h"

// AI States
#include "AIStateIdle.h"
#include "AIStateChasing.h"
#include "AIStateWander.h"
#include "AIStateChasingFast.h"
#include "AIStateGrabbingCoin1.h"
#include "AIStateGrabbingCoin2.h"

#if defined(_WIN32)
// For OutputDebugString
#include "windows.h"
#else
#include <iostream>
#endif

using namespace week9;

//------------------------------------------------------------------------------
// Method:    ComponentAIController
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
ComponentAIController::ComponentAIController()
	:
	m_pDecisionTree(NULL),
	m_pStateMachine(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~ComponentAIController
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ComponentAIController::~ComponentAIController()
{
	if (m_pDecisionTree)
	{
		delete m_pDecisionTree;
		m_pDecisionTree = NULL;
	}

	if (m_pStateMachine)
	{
		m_pStateMachine->SetStateMachineOwner(NULL);
		delete m_pStateMachine;
		m_pStateMachine = NULL;
	}
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   void
// 
// Polls input and sends movement instructions to relevant sibling components.
//------------------------------------------------------------------------------
void ComponentAIController::Update(float p_fDelta)
{
	const std::string& strBestAction = m_pDecisionTree->Decide();
    
#if defined(_WIN32)
	char buff[128];
	sprintf(buff, "ACTION = [%s]\n", strBestAction.c_str());
	OutputDebugString(buff);
#else
    std::cout << "ACTION = [" << strBestAction << ']' << std::endl;
#endif

	// Do we need to switch states?
	AIState eBestState = this->MapActionToState(strBestAction);
	if (m_pStateMachine->GetCurrentState() != eBestState)
	{
		m_pStateMachine->GoToState(eBestState);
	}

	m_pStateMachine->Update(p_fDelta);
}

//------------------------------------------------------------------------------
// Method:    Init
// Parameter: const char * p_strDecisionTreePath
// Returns:   void
// 
// Initializes the AI controller using a decision tree defined in XML.
//------------------------------------------------------------------------------
void ComponentAIController::Init(const char* p_strDecisionTreePath)
{
	// Create our decision tree to manage which states we go into
	m_pDecisionTree = new AIDecisionTree(this);
	m_pDecisionTree->Load(p_strDecisionTreePath);

	// Initialize the StateMachine and supported states
	m_pStateMachine = new Common::StateMachine();
	m_pStateMachine->SetStateMachineOwner(this);
	m_pStateMachine->RegisterState(eAIState_Idle, new AIStateIdle());
	m_pStateMachine->RegisterState(eAIState_Wander, new AIStateWander());
	m_pStateMachine->RegisterState(eAIState_Chasing, new AIStateChasing());
	m_pStateMachine->RegisterState(eAIState_GrabbingCoin1, new AIStateGrabbingCoin1());
	m_pStateMachine->RegisterState(eAIState_GrabbingCoin2, new AIStateGrabbingCoin2());
}