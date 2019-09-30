//------------------------------------------------------------------------
// AIDecisionTree
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class loads an AI decision tree into memory.
//------------------------------------------------------------------------

#ifndef AIDECISIONTREE_H
#define AIDECISIONTREE_H

#include "tinyxml.h"
#include "AIActionNode.h"
#include "AIDecisionNode.h"
#include "AINode.h"

namespace week9
{
	// Forward declaration
	class ComponentAIController;

	class AIDecisionTree
	{
	public:
		//------------------------------------------------------------------------------
		// Public types.
		//------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIDecisionTree(ComponentAIController* p_pAIController);
		~AIDecisionTree();

		bool Load(const char* p_strDecisionTreePath);
		const std::string& Decide();

		ComponentAIController* GetController() { return m_pAIController; }

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// The root AI node
		AINode* m_pRootNode;

		// The owning controller
		ComponentAIController* m_pAIController;
	};
}

#endif // AIDECISIONTREE_H

