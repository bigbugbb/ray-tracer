#pragma once
#include "primitive.h"

class CPlane : public CPrimitive
{
public:
	CPlane(void);
	virtual ~CPlane(void);

	void SetNormal(M3DVector3f vPosition);
	void SetPosition(M3DVector3f vPoint);

	virtual void GetNormal(M3DVector3f vNormal, M3DVector3f vPoint);
	virtual BOOL Intersect(const M3DVector3f vRayOrgin, const M3DVector3f vRayDir, GLfloat fMinT, GLfloat fMaxT);

protected:
	M3DVector3f		m_vNormal;
	M3DVector3f		m_vPosition;
	GLfloat			m_fCurT;
};

