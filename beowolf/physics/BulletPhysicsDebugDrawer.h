//------------------------------------------------------------------------
// BulletPhysicsDebugDrawer
//
// Created:	2013/02/18
// Author:	Carel Boers
//	
// Debug drawer for the BulletPhysicsManager.
//------------------------------------------------------------------------

#ifndef BULLETPHYSICSDEBUGDRAWER_H
#define BULLETPHYSICSDEBUGDRAWER_H

#include "LinearMath/btIDebugDraw.h"
#include "W_LineDrawer.h"

class BulletPhysicsDebugDrawer : public btIDebugDraw
{
public:
	//-------------------------------------------------------------------------
	// Public methods.
	//-------------------------------------------------------------------------
	BulletPhysicsDebugDrawer(wolf::LineDrawer* p_pLineDrawer);
	virtual ~BulletPhysicsDebugDrawer(); 

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual void	drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
	virtual void	drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);
	virtual void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	virtual void	reportErrorWarning(const char* warningString);
	virtual void	draw3dText(const btVector3& location, const char* textString);
	virtual void	setDebugMode(int debugMode);
	virtual int		getDebugMode() const { return m_debugMode;}

private:
	//-------------------------------------------------------------------------
	// Private members.
	//-------------------------------------------------------------------------

	// Debug mode
	int m_debugMode;

	// Line drawer to help render diagnostics
	wolf::LineDrawer* m_pLineDrawer;
};

#endif //BULLETPHYSICSDEBUGDRAWER_H
