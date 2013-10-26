#include "stdafx.h"
#include "Sphere.h"


CSphere::CSphere(void)
{
	m_eType = SPHERE;
}

CSphere::~CSphere(void)
{
}

void CSphere::SetCenter(const M3DVector3f vCenter)
{

	m_vCenter[0] = vCenter[0];
	m_vCenter[1] = vCenter[1];
	m_vCenter[2] = vCenter[2];
}

void CSphere::SetCenter(GLfloat x, GLfloat y, GLfloat z)
{
	m_vCenter[0] = x;
	m_vCenter[1] = y;
	m_vCenter[2] = z;
}

void CSphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

void CSphere::GetNormal(M3DVector3f vNormal, M3DVector3f vPoint)
{
	m3dSubtractVectors3(vNormal, vPoint, m_vCenter);
	m3dNormalizeVector3(vNormal);
}

BOOL CSphere::Intersect(const M3DVector3f vRayOrgin, const M3DVector3f vRayDir, GLfloat fMinT, GLfloat fMaxT)
{
	BOOL bFound = TRUE;
	GLfloat fDiscriminant;
	GLfloat fT1, fT2;

	// calculate the discriminant
	M3DVector3f vEminusC;
	GLfloat fA, fB, fC;
	m3dSubtractVectors3(vEminusC, vRayOrgin, m_vCenter);
	fA = m3dDotProduct3(vRayDir, vRayDir);
	fB = m3dDotProduct3(vRayDir, vEminusC); // ignore 4, only check b * b - a * c
	fC = m3dDotProduct3(vEminusC, vEminusC) - m_fRadius * m_fRadius;
	fDiscriminant = fB * fB - fA * fC;

	// calculate t according to the discriminant
	if (fDiscriminant > 0) {
		GLfloat fTemp = sqrt(fDiscriminant);
		fT1 = (-fB + fTemp) / fA;
		fT2 = (-fB - fTemp) / fA;
		m_vecHitNote.push_back(min(fT1, fT2));
		m_vecHitNote.push_back(max(fT1, fT2));
	} else if (fDiscriminant == 0) {		
		fT1 = -fB / fA;
		m_vecHitNote.push_back(fT1);
	} else {
		bFound = FALSE;
	}

	return bFound;
}