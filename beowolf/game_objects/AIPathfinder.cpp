//------------------------------------------------------------------------
// AIPathfinder
//
// Created:	2012/12/14
// Author:	Carel Boers
//	
// This class manages finding paths given a set of traversable nodes.
//------------------------------------------------------------------------

#include "AIPathfinder.h"
#include "ComponentRenderableMesh.h"
#include "W_ResourceLoader.h"
#include <iostream>
#include <fstream>
#if defined(_WIN32)
#include "windows.h"
#else
#include <iostream>
#endif

using namespace week9;

// Static singleton instance
AIPathfinder* AIPathfinder::s_pPathfindingInstance = NULL;

//------------------------------------------------------------------------------
// Method:    CreateInstance
// Returns:   void
// 
// Creates the singletone instance.
//------------------------------------------------------------------------------
void AIPathfinder::CreateInstance()
{
	//assert(s_pPathfindingInstance == NULL);
	s_pPathfindingInstance = new AIPathfinder();
}

//------------------------------------------------------------------------------
// Method:    DestroyInstance
// Returns:   void
// 
// Destroys the singleton instance.
//------------------------------------------------------------------------------
void AIPathfinder::DestroyInstance()
{
	assert(s_pPathfindingInstance != NULL);
	delete s_pPathfindingInstance;
	s_pPathfindingInstance = NULL;
}

//------------------------------------------------------------------------------
// Method:    Instance
// Returns:   SceneManager::SceneManager*
// 
// Access to singleton instance.
//------------------------------------------------------------------------------
AIPathfinder* AIPathfinder::Instance()
{
	assert(s_pPathfindingInstance);
	return s_pPathfindingInstance;
}


//------------------------------------------------------------------------------
// Method:    AIPathfinder
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
AIPathfinder::AIPathfinder()
	:
	m_bDebugRendering(false),
	m_pLineDrawer(NULL)
{
	m_pLineDrawer = new wolf::LineDrawer();
	m_pLineDrawer->Init(wolf::ResourceLoader::Instance().getVertexShader("lines.vsh"), wolf::ResourceLoader::Instance().getPixelShader("lines.fsh"));
}

//------------------------------------------------------------------------------
// Method:    ~AIPathfinder
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
AIPathfinder::~AIPathfinder()
{
	NodeList::iterator it = m_lPathNodes.begin(), end = m_lPathNodes.end();
	for (; it != end; ++it)
	{
		PathNode* pNode = static_cast<PathNode*>(*it);
		delete pNode;
		pNode = NULL;
	}	
	m_lPathNodes.clear();

	if (m_pLineDrawer)
	{
		delete m_pLineDrawer;
		m_pLineDrawer = NULL;
	}
}

//------------------------------------------------------------------------------
// Method:    Load
// Parameter: const char * p_strPathfindingData
// Returns:   bool
// 
// Loads the pathfinding data for a given level.
//------------------------------------------------------------------------------
bool AIPathfinder::Load(const char* p_strPathfindingData)
{
	std::ifstream inFile(p_strPathfindingData);

	json p_path = json::parse(inFile);

	size_t it_h = 0;

	auto& nodes = p_path["Paths"]["Nodes"];

	for (auto& node : nodes)
	{
		std::string IDStr = node["ID"];
		std::string xstr = node["X"];
		std::string ystr = node["Y"];
		std::string zstr = node["Z"];

		int idx = std::stoi(IDStr);
		float x = std::stof(xstr);
		float y = std::stof(ystr);
		float z = std::stof(zstr);

		//std::cout << idx << ": " << x << ", " << y << ", " << z << std::endl;

		// Add the node to our list
		PathNode* pNode = new PathNode();
		pNode->m_vPosition = glm::vec3(x, y, z);
		m_lPathNodes.push_back(pNode);
		//assert(idx == (m_lPathNodes.size() - 1));
	}

	auto& arcs = p_path["Paths"]["Arcs"];
	for (auto& arc : arcs)
	{
		std::string startN = arc["start"];
		std::string endN = arc["end"];
			
		int start = std::stoi(startN);
		int end = std::stoi(endN);

		//std::cout << start << ", " << end << std::endl;

		//assert(start >= 0 && start < m_lPathNodes.size());
		//assert(end >= 0 && end < m_lPathNodes.size());

		PathNode* pNode1 = m_lPathNodes[start];
		PathNode* pNode2 = m_lPathNodes[end];
		pNode1->m_lNeighbourNodes.push_back(pNode2);
		pNode2->m_lNeighbourNodes.push_back(pNode1);
	}

	return true;
}

//------------------------------------------------------------------------------
// Method:    GetClosestNode
// Parameter: const glm::vec3 & p_vPosition
// Returns:   const AIPathfinder::PathNode*
//
// Returns the path node closest to the given position.
//------------------------------------------------------------------------------
const AIPathfinder::PathNode* const AIPathfinder::GetClosestNode(const glm::vec3& p_vPosition)
{
	PathNode* pNode = NULL;
	float fNodeDist = 10000.0f;

	// Find the node closest to the given position
	NodeList::const_iterator it = m_lPathNodes.begin(), end = m_lPathNodes.end();
	for (; it != end; ++it)
	{
		PathNode* pTemp = static_cast<PathNode*>(*it);
		float fTempDist = glm::length(pTemp->m_vPosition - p_vPosition);

		if (pNode == NULL || fTempDist < fNodeDist)
		{
			pNode = pTemp;
			fNodeDist = fTempDist;
		}
	}

	return pNode;
}

//------------------------------------------------------------------------------
// Method:    FindPath
// Parameter: const glm::vec3 & p_vStart
// Parameter: const glm::vec3 & p_vDestination
// Returns:   void
// 
// Finds a path to the given destination if a path exists. Returns a reference
// to the local path node list; so if you want to use this value across multiple
// frames; make your own copy.
//------------------------------------------------------------------------------
const AIPathfinder::PositionList& AIPathfinder::FindPath(const glm::vec3& p_vStart, const glm::vec3& p_vDestination)
{
	PathNode* pStartNode = NULL;
	PathNode* pEndNode = NULL;
	float fDistance = glm::length(p_vStart - p_vDestination);
	float fStartNodeDist = 10000.0f;
	float fEndNodeDist = 10000.0f;
	char strDebugBuffer[64];

	// Find the start and end nodes; we'll just use the nodes that are closest to the start/destination positions
	NodeList::const_iterator it = m_lPathNodes.begin(), end = m_lPathNodes.end();
	for (; it != end; ++it)
	{
		PathNode* pNode = static_cast<PathNode*>(*it);

		// Reset cached values
		pNode->pParentNode = NULL;
		pNode->bInClosedList = false;
		pNode->bInOpenList = false;
		pNode->G = 0;
		pNode->H = 0;

		// Skip nodes without any neighbours
		if (pNode->m_lNeighbourNodes.size() < 1)
		{
			continue;
		}

		float fDistToStartPos = glm::length(pNode->m_vPosition - p_vStart);
		float fDistToEndPos = glm::length(pNode->m_vPosition - p_vDestination);

		if (pStartNode == NULL || fDistToStartPos < fStartNodeDist || IsNodeBlocked(pStartNode->m_vPosition))
		{
			pStartNode = pNode;
			fStartNodeDist = fDistToStartPos;
		}
		if (pEndNode == NULL || fDistToEndPos < fEndNodeDist || IsNodeBlocked(pStartNode->m_vPosition))
		{
			pEndNode = pNode;
			fEndNodeDist = fDistToEndPos;
		}
	}

	// The open list is all the nodes we're considering as options for our path
	std::vector<PathNode*> lOpenList;
	std::vector<PathNode*> lClosedList;

	// Start by calculating our node costs for the start node and add it to the open list
	pStartNode->pParentNode = NULL;
	pStartNode->bInOpenList = true;
	pStartNode->bInClosedList = false;
	pStartNode->G = 0;																	// No cost since we're starting here
	pStartNode->H = glm::length(pStartNode->m_vPosition - pEndNode->m_vPosition);		// "As the crow flies" cost to get to the end square
	lOpenList.push_back(pStartNode);

	// Loop until we find a path using the A* search algorithm
	bool bGoalFound = false;
	PathNode* pCurrentNode = NULL;
	while (!bGoalFound)
	{
		// Make sure the open list is not empty
		if (lOpenList.empty())
		{
			break;
		}

		// Find the node in the open list with the lowest F cost
		pCurrentNode = NULL;
		NodeList::iterator openListIt = lOpenList.begin(), openListEnd = lOpenList.end();
		NodeList::iterator bestIt = openListIt;
		for (; openListIt != openListEnd; ++openListIt)
		{
			PathNode* pTemp = static_cast<PathNode*>(*openListIt);
			if (pCurrentNode == NULL || (pTemp->G + pTemp->H) < (pCurrentNode->G + pCurrentNode->H) || IsNodeBlocked(pCurrentNode->m_vPosition))
			{
				pCurrentNode = pTemp;
				bestIt = openListIt;
			}
		}

		// Drop this node from the open list and add it to our closed list
		lOpenList.erase(bestIt);
		lClosedList.push_back(pCurrentNode);
		pCurrentNode->bInOpenList = false;
		pCurrentNode->bInClosedList = true;

#if defined(_WIN32)
		sprintf(strDebugBuffer, "Closed List: Adding [%f, %f]\n", pCurrentNode->m_vPosition.x, pCurrentNode->m_vPosition.z);
		OutputDebugString(strDebugBuffer);
#else
        std::cout << "Closed List: Adding [" << pCurrentNode->m_vPosition.x << ", " << pCurrentNode->m_vPosition.z << ']' << std::endl;
#endif


		// If this is our goal node, we're done.
		if (pCurrentNode == pEndNode)
		{
			bGoalFound = true;
			break;
		}

		// Add all the neighbours of our current node to the open list
		NodeList::iterator it2 = pCurrentNode->m_lNeighbourNodes.begin(), end2 = pCurrentNode->m_lNeighbourNodes.end();
		for (; it2 != end2; ++it2)
		{
			PathNode* pNeighbour = static_cast<PathNode*>(*it2);
			if (pNeighbour->bInClosedList || IsNodeBlocked(pNeighbour->m_vPosition))
			{
				// Skip
				continue;
			}
			else if (pNeighbour->bInOpenList)
			{
				// See if getting to this node from our current node is a better path
				int currentGCost = pNeighbour->G;
				int newGCost;
				//CHECK IF NEIGHBOUR NODE IS SAND, DOUBLE IF SO
				if (WithinDesert(pNeighbour->m_vPosition))
				{
					newGCost = pCurrentNode->G + 2* (glm::length(pCurrentNode->m_vPosition - pNeighbour->m_vPosition)); // Parent cost + "as the crow flies" cost to get from parent to us
				}
				else
				{
					newGCost = pCurrentNode->G + glm::length(pCurrentNode->m_vPosition - pNeighbour->m_vPosition); // Parent cost + "as the crow flies" cost to get from parent to us
				}

				if (newGCost < currentGCost)
				{
					// Change parent; we found a better route
					pNeighbour->pParentNode = pCurrentNode;
					pNeighbour->G = newGCost;
				}
			}
			else
			{
				// New; add it to your list
				pNeighbour->pParentNode = pCurrentNode;
				pNeighbour->bInOpenList = true;
				pNeighbour->bInClosedList = false;
				if (WithinDesert(pNeighbour->m_vPosition))
				{
					pNeighbour->G = pCurrentNode->G + 2*(glm::length(pCurrentNode->m_vPosition - pNeighbour->m_vPosition));	// Parent cost + "as the crow flies" cost to get from parent to us
				}
				else
				{
					pNeighbour->G = pCurrentNode->G + glm::length(pCurrentNode->m_vPosition - pNeighbour->m_vPosition);	// Parent cost + "as the crow flies" cost to get from parent to us
				}
				
				//CHECK IF NEIGHBOUR NODE IS SAND, DOUBLE IF SO
				
				pNeighbour->H = glm::length(pNeighbour->m_vPosition - pEndNode->m_vPosition);			// "As the crow flies" cost to get to the end square
				lOpenList.push_back(pNeighbour);

#if defined(_WIN32)
				sprintf(strDebugBuffer, "Open List: Adding [%f, %f]\n", pNeighbour->m_vPosition.x, pNeighbour->m_vPosition.z);
                OutputDebugString(strDebugBuffer);
#else
                std::cout << "Open List: Adding [" << pNeighbour->m_vPosition.x << ", " << pNeighbour->m_vPosition.z << ']' << std::endl;
#endif
			}
		}
	}

	// Make sure we found the path
	m_lLastPath.clear();
	if (bGoalFound && pCurrentNode)
	{
		// Using our current (goal) node; traverse each node by looking to the next parent to find our path
		PathNode* pNode = pCurrentNode;
		while (pNode)
		{
			m_lLastPath.push_front(pNode->m_vPosition);
			pNode = pNode->pParentNode;
		}

		// Debugging path points
		PositionList::iterator it = m_lLastPath.begin(), end = m_lLastPath.end();
		for (; it != end; ++it)
		{
			glm::vec3 vPos = static_cast<glm::vec3>(*it);

#if defined(_WIN32)
			char buff[64];
			sprintf(buff, "POS[%f, %f, %f]\n", vPos.x, vPos.y, vPos.z);
			OutputDebugString(buff);
#else
            std::cout << "POS[" << vPos.x << ", " << vPos.y << ", " << vPos.z << ']' << std::endl;
#endif
		}
	}
	
	return m_lLastPath;
}

//------------------------------------------------------------------------------
// Method:    ToggleDebugRendering
// Parameter: GameObjectManager * p_pGameObjectManager
// Returns:   void
// 
// Toggles debug rendering on/off.
//------------------------------------------------------------------------------
void AIPathfinder::ToggleDebugRendering(Common::GameObjectManager* p_pGameObjectManager)
{
	m_bDebugRendering = !m_bDebugRendering;

	char goNameBuffer[64];
	if (!m_bDebugRendering)
	{
		// Delete all node game objects
		for (int i = 0; i < m_lPathNodes.size(); ++i)
		{
			sprintf(goNameBuffer, "Node%d", i);
			Common::GameObject* pNode = p_pGameObjectManager->GetGameObject(goNameBuffer);
			p_pGameObjectManager->DestroyGameObject(pNode);
		}
	}
	else
	{
		// Add and create a game object for each node
		for (int i = 0; i < m_lPathNodes.size(); ++i)
		{
			sprintf(goNameBuffer, "Node%d", i);
			Common::GameObject* pNode = p_pGameObjectManager->CreateGameObject();
			week2::ComponentRenderableMesh* pRenderableComponent = new week2::ComponentRenderableMesh();
			pRenderableComponent->Init("assignmentResources/assignment4/node.pod", "assignmentResources/assignment4/data/", wolf::ResourceLoader::Instance().getVertexShader("textured.vsh"), wolf::ResourceLoader::Instance().getPixelShader("textured.fsh"));
			pNode->AddComponent(pRenderableComponent);
			p_pGameObjectManager->SetGameObjectGUID(pNode, goNameBuffer);

			// Set position
			PathNode* pPathNode = m_lPathNodes[i];
			pNode->GetTransform().SetTranslation(pPathNode->m_vPosition);
		}
	}
}

//------------------------------------------------------------------------------
// Method:    Render
// Parameter: const glm::mat4 & p_mProj
// Parameter: const glm::mat4 & p_mView
// Returns:   void
// 
// Debug rendering of AI pathfinding structure
//------------------------------------------------------------------------------
void AIPathfinder::Render(const glm::mat4& p_mProj, const glm::mat4& p_mView)
{
	if (m_bDebugRendering)
	{
		// Add lines for each connected node pair
		NodeList::const_iterator it = m_lPathNodes.begin(), end = m_lPathNodes.end();
		for (; it != end; ++it)
		{
			PathNode* pNode1 = static_cast<PathNode*>(*it);

			// Iterator neighbours
			NodeList::const_iterator it2 = pNode1->m_lNeighbourNodes.begin(), end2 = pNode1->m_lNeighbourNodes.end();
			for (; it2 != end2; ++it2)
			{
				PathNode* pNode2 = static_cast<PathNode*>(*it2);
				m_pLineDrawer->AddLine(pNode1->m_vPosition, pNode2->m_vPosition, wolf::Color4(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}

		// If a path has been found; render it too
		if (m_lLastPath.size() > 0)
		{
			bool bHasLastPos = false;
			glm::vec3 vLastPos;
			PositionList::iterator it = m_lLastPath.begin(), end = m_lLastPath.end();
			for (; it != end; ++it)
			{
				glm::vec3 vPos = static_cast<glm::vec3>(*it);
				if (bHasLastPos)
				{
					m_pLineDrawer->AddLine(vLastPos, vPos, wolf::Color4(1.0f, 0.0f, 0.0f, 1.0f));
				}
				vLastPos = vPos;
				bHasLastPos = true;

			}
		}

		m_pLineDrawer->Render(p_mProj, p_mView);
	}
}

bool AIPathfinder::IsNodeBlocked(glm::vec3 nodePos)
{
	for (int i = 0; i < m_blockedNodes.size(); i++)
	{
		if (glm::length(m_blockedNodes[i] - nodePos) < 0.01)
			return true;
	}
	
	return false;
}

void AIPathfinder::BlockNode(glm::vec3 nodePos)
{
	m_blockedNodes.push_back(nodePos);
}

void AIPathfinder::ClearBlockedNodes()
{
	m_blockedNodes.clear();
}

void AIPathfinder::SetDesertPositions(std::vector<glm::vec3> desert)
{
	desertPos = desert;
}

bool AIPathfinder::WithinDesert(glm::vec3 check)
{
	for (int i = 0; i < desertPos.size(); i++)
	{
		if (glm::length(check - desertPos.at(i)) < 0.01)
			return true;
	}
	return false;
}