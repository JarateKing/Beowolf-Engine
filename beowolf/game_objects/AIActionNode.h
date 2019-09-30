//------------------------------------------------------------------------
// AIActionNode
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class represents an action (leaf) node in an AI decision tree.
//------------------------------------------------------------------------

#ifndef AIACTIONNODE_H
#define AIACTIONNODE_H

#include "tinyxml.h"
#include "AINode.h"

namespace week9
{
	class AIActionNode : public AINode
	{
	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIActionNode(AIDecisionTree* p_pTree);
		virtual ~AIActionNode();

		virtual bool Init(TiXmlNode* p_pNodeDefinition);
		virtual const std::string &Decide();

	private:
		//------------------------------------------------------------------------------
		// Private members.

		std::string m_strAction;
	};
}

#endif // AIDECISIONTREE_H

