//------------------------------------------------------------------------
// AINode
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class represents a node in an AI decision tree.
//------------------------------------------------------------------------

#ifndef AINODE_H
#define AINODE_H

#include "tinyxml.h"
#include <string>

namespace week9
{
	// Forward declaration
	class AIDecisionTree;
	class AINode
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AINode(AIDecisionTree* p_pTree) : m_pTree(p_pTree) {}
		virtual ~AINode() {}

		virtual bool Init(TiXmlNode* p_pNodeDefinition) = 0;
		virtual const std::string &Decide() = 0;

	protected:
		//------------------------------------------------------------------------------
		// Protected members.
		//------------------------------------------------------------------------------

		// The tree this node belongs to.
		AIDecisionTree* m_pTree;
	};
}

#endif // AINODE_H

