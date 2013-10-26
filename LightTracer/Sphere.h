#pragma once
#include "primitive.h"

class CSphere : public CPrimitive
{
public:
	CSphere(void);
	virtual ~CSphere(void);

	void SetCenter(const M3DVector3f vCenter);
	void SetCenter(GLfloat x, GLfloat y, GLfloat z);
	void SetRadius(GLfloat fRadius);

	virtual void GetNormal(M3DVector3f vNormal, M3DVector3f vPoint);
	virtual BOOL Intersect(const M3DVector3f vRayOrgin, const M3DVector3f vRayDir, GLfloat fMinT, GLfloat fMaxT);

protected:
	GLfloat			m_fRadius;
	M3DVector3f		m_vCenter;
};

