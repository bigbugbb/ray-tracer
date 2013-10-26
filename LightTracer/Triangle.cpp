#include "stdafx.h"
#include "Triangle.h"


CTriangle::CTriangle(void)
{
	m_eType = TRIANGLE;
}

CTriangle::~CTriangle(void)
{
}

void CTriangle::SetPosition(M3DVector3f point1, M3DVector3f point2, M3DVector3f point3)
{
	m3dCopyVector3(m_vPoint1, point1);
	m3dCopyVector3(m_vPoint2, point2);
	m3dCopyVector3(m_vPoint3, point3);
}

void CTriangle::SetNormal(M3DVector3f vNormal)
{
	m3dCopyVector3(m_vNormal, vNormal);	
	m3dNormalizeVector3(m_vNormal);
}

void CTriangle::GetNormal(M3DVector3f vNormal, M3DVector3f vPoint)
{
	m3dCopyVector3(vNormal, m_vNormal);
}

BOOL CTriangle::Intersect(const M3DVector3f vRayOrgin, const M3DVector3f vRayDir, GLfloat fMinT, GLfloat fMaxT)
{
	M3DVector3f vSubP1P2;// Point1 - Point2
	M3DVector3f vSubP1P3;// Point1 - Point3
	M3DVector3f vSubP1E; // Point1 - RayOrgin

	m3dSubtractVectors3(vSubP1P2, m_vPoint1, m_vPoint2);
	m3dSubtractVectors3(vSubP1P3, m_vPoint1, m_vPoint3);
	m3dSubtractVectors3(vSubP1E,  m_vPoint1, vRayOrgin);

	// vSubP1P2 = [a b c]
	// vSubP1P3 = [d e f]
	// vRayDir  = [g h i]
	// vSubP1E  = [j k l]
	//  a  d  g    beta     j
	//  b  e  h *  gama  =  k
	//  c  f  i      t      l
	float eihf = vSubP1P3[1] * vRayDir[2] - vRayDir[1] * vSubP1P3[2];
	float gfdi = vSubP1P3[2] * vRayDir[0] - vRayDir[2] * vSubP1P3[0];
	float dheg = vSubP1P3[0] * vRayDir[1] - vRayDir[0] * vSubP1P3[1];

	float akjb = vSubP1P2[0] * vSubP1E[1] - vSubP1E[0] * vSubP1P2[1];
	float jcal = vSubP1P2[2] * vSubP1E[0] - vSubP1E[2] * vSubP1P2[0];
	float blkc = vSubP1P2[1] * vSubP1E[2] - vSubP1E[1] * vSubP1P2[2];

	float fM = vSubP1P2[0] * eihf + vSubP1P2[1] * gfdi + vSubP1P2[2] * dheg;
	float fT = -(vSubP1P3[2] * akjb + vSubP1P3[1] * jcal + vSubP1P3[0] * blkc) / fM;
	if (fT < fMinT || fT > fMaxT) {
		return FALSE;
	} else {
		float fGama = (vRayDir[2] * akjb + vRayDir[1] * jcal + vRayDir[0] * blkc) / fM;
		if (fGama < 0.0f || fGama > 1.0f) {
			return FALSE;
		} else {
			float fBeta  = (vSubP1E[0] * eihf + vSubP1E[1] * gfdi + vSubP1E[2] * dheg) / fM;
			if (fBeta < 0.0f || fBeta > 1.0f - fGama) {			
				return FALSE;
			} else {
				m_vecHitNote.push_back(fT);
				return TRUE;
			}
		}
	}
	return FALSE;
}