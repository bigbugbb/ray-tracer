#pragma once
#include "primitive.h"

class CTriangle : public CPrimitive
{
public:
	CTriangle(void);
	virtual ~CTriangle(void);

	void SetNormal(M3DVector3f vPosition);
	void SetPosition(M3DVector3f point1, M3DVector3f point2, M3DVector3f point3);

	virtual void GetNormal(M3DVector3f vNormal, M3DVector3f vPoint);
	virtual BOOL Intersect(const M3DVector3f vRayOrgin, const M3DVector3f vRayDir, GLfloat fMinT, GLfloat fMaxT);

protected:
	M3DVector3f		m_vNormal;
	M3DVector3f		m_vPoint1;
	M3DVector3f		m_vPoint2;
	M3DVector3f		m_vPoint3;
};

