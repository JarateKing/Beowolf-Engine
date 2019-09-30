//------------------------------------------------------------------------
// BulletPhysicsDebugDrawer
//
// Created:	2013/02/18
// Author:	Carel Boers
//	
// Debug drawer for the BulletPhysicsManager.
//------------------------------------------------------------------------

#include "BulletPhysicsDebugDrawer.h"

//------------------------------------------------------------------------------
// Method:    BulletPhysicsDebugDrawer
// Parameter: wolf::LineDrawer * p_pLineDrawer
// Returns:   
// 
// Constructor.
//------------------------------------------------------------------------------
BulletPhysicsDebugDrawer::BulletPhysicsDebugDrawer(wolf::LineDrawer* p_pLineDrawer)
	:
	m_debugMode(0),
	m_pLineDrawer(p_pLineDrawer)
{
}

//------------------------------------------------------------------------------
// Method:    ~BulletPhysicsDebugDrawer
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
BulletPhysicsDebugDrawer::~BulletPhysicsDebugDrawer()
{
}

//------------------------------------------------------------------------------
// Method:    drawLine
// Parameter: const btVector3 & from
// Parameter: const btVector3 & to
// Parameter: const btVector3 & fromColor
// Parameter: const btVector3 & toColor
// Returns:   void
// 
// Draws the given line defined by a start and end point and colors.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
{
	m_pLineDrawer->AddLine(glm::vec3(from.getX(), from.getY(), from.getZ()),
							glm::vec3(to.getX(), to.getY(), to.getZ()),
							wolf::Color4(fromColor.getX(), fromColor.getY(), fromColor.getZ(), 1.0f));
}

//------------------------------------------------------------------------------
// Method:    drawLine
// Parameter: const btVector3 & from
// Parameter: const btVector3 & to
// Parameter: const btVector3 & color
// Returns:   void
// 
// Draws the given line defined by a start and end point and colors.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	this->drawLine(from, to, color, color);
}

//------------------------------------------------------------------------------
// Method:    drawSphere
// Parameter: const btVector3 & p
// Parameter: btScalar radius
// Parameter: const btVector3 & color
// Returns:   void
// 
// Draw a sphere.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
{
	// TODO: implement
}

//------------------------------------------------------------------------------
// Method:    drawTriangle
// Parameter: const btVector3 & a
// Parameter: const btVector3 & b
// Parameter: const btVector3 & c
// Parameter: const btVector3 & color
// Parameter: btScalar alpha
// Returns:   void
// 
// Draw a triangle.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
{
	// TODO: implement
}

//------------------------------------------------------------------------------
// Method:    setDebugMode
// Parameter: int debugMode
// Returns:   void
// 
// Set debug mode.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

//------------------------------------------------------------------------------
// Method:    draw3dText
// Parameter: const btVector3 & location
// Parameter: const char * textString
// Returns:   void
// 
// Draw 3D text.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
	// TODO: implement
}

//------------------------------------------------------------------------------
// Method:    reportErrorWarning
// Parameter: const char * warningString
// Returns:   void
// 
// Error reporting callback.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	 // TODO: implement
}

//------------------------------------------------------------------------------
// Method:    drawContactPoint
// Parameter: const btVector3 & pointOnB
// Parameter: const btVector3 & normalOnB
// Parameter: btScalar distance
// Parameter: int lifeTime
// Parameter: const btVector3 & color
// Returns:   void
// 
// Draw a contact point.
//------------------------------------------------------------------------------
void BulletPhysicsDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
	btVector3 to = pointOnB + normalOnB * distance;
	m_pLineDrawer->AddLine(glm::vec3(pointOnB.getX(), pointOnB.getY(), pointOnB.getZ()),
							glm::vec3(to.getX(), to.getY(), to.getZ()),
							wolf::Color4(color.getX(), color.getY(), color.getZ(), 1.0f));
}





