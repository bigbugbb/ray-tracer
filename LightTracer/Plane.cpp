#include "stdafx.h"
#include "Plane.h"


CPlane::CPlane(void)
{
	m_eType = PLANE;
}

CPlane::~CPlane(void)
{
}

void CPlane::SetPosition(M3DVector3f vPosition)
{
	m3dCopyVector3(m_vPosition, vPosition);
}

void CPlane::SetNormal(M3DVector3f vNormal)
{
	m3dCopyVector3(m_vNormal, vNormal);	
	m3dNormalizeVector3(m_vNormal);
}

void CPlane::GetNormal(M3DVector3f vNormal, M3DVector3f vPoint)
{
	m3dCopyVector3(vNormal, m_vNormal);
}

BOOL CPlane::Intersect(const M3DVector3f vRayOrgin, const M3DVector3f vRayDir, float fMinT, float fMaxT)
{
	M3DVector3f vTemp;

	// first ignore rays that are perpendicular to the plane
	if (m3dDotProduct3(m_vNormal, vRayDir) == 0) {
		return FALSE;
	}
	// find the t
	m3dSubtractVectors3(vTemp, m_vPosition, vRayOrgin);
	GLfloat fNumerator   = m3dDotProduct3(m_vNormal, vTemp);
	GLfloat fDenominator = m3dDotProduct3(m_vNormal, vRayDir);
	GLfloat t = fNumerator / fDenominator;
	// ignore t if it's bigger than fMaxT or smaller than fMinT
	if (t < fMinT || t > fMaxT) {
		return FALSE;
	}
	// record current t
	m_vecHitNote.push_back(t);

	return TRUE;
}
