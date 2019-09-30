//------------------------------------------------------------------------
// AIDecisionNode
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class represents a decision node in an AI decision tree.
//------------------------------------------------------------------------

#ifndef AIDECISIONNODE_H
#define AIDECISIONNODE_H

#include "tinyxml.h"
#include "AINode.h"

namespace week9
{
	class AIDecisionNode : public AINode
	{
	private:
		//------------------------------------------------------------------------------
		// Private types
		//------------------------------------------------------------------------------

		// The type of decision node; this determines how we evaluate whether we choose the 
		// left or right (true or false) child node when Decide() is called.
		// NOTE: we'd probably be better off having each type of decision as a separate class; it 
		//		would scale much better.
		enum eDecisionType
		{
			eDecision_NONE = -1,
			eDecisionType_ProximityToGO,
			eDecisionType_StateTimer,
			eDecisionType_WeightedRandom,
			eDecisionType_StatePrecondition,
			eDecisionType_IsClosestToGO
		};

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		AIDecisionNode(AIDecisionTree* p_pTree);
		virtual ~AIDecisionNode();

		virtual bool Init(TiXmlNode* p_pNodeDefinition);
		virtual const std::string &Decide();

		// Decision type evaluations
		bool EvaluateProximityDecision();
		bool EvaluateTimerDecision();
		bool EvaluateWeightedRandomDecision();
		bool EvaluateStatePreconditionDecision();
		bool EvaluateClosestDecision();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Left and right children
		AINode* m_pTrueNode;
		AINode* m_pFalseNode;

		// Decision Type
		eDecisionType m_eDecisionType;

		// Decision data
		// NOTE: these values are generic and contain different things depending on the type of decision
		//		we are making (i.e. the value of m_eDecisionType).
		//		Again, this would be cleaner with subclasses that can contain the data they need to make 
		//		their own decision.
		std::string m_strName;
		int m_iThreshold;
	};
}

#endif // AIDECISIONNODE_H

