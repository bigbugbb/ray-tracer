#include "stdafx.h"
#include "PointLight.h"


CPointLight::CPointLight(void)
{
	m_eType = LightType::POINT_LIGHT;

	m_vAttenuation[0] = 0;
	m_vAttenuation[1] = 0;
	m_vAttenuation[2] = 0.0005;
}


CPointLight::~CPointLight(void)
{
}

void CPointLight::GetDirection(M3DVector3f vDirection, M3DVector3f vHitPoint)
{
	m3dSubtractVectors3(vDirection, vHitPoint, m_vPosition);
	m3dNormalizeVector3(vDirection);
}

void CPointLight::CalculateIntensity(M3DVector3f vIntensity, M3DVector3f vHitPoint)
{
	GLfloat fDist, fDistSquare;
	M3DVector3f vDist;

	m3dSubtractVectors3(vDist, vHitPoint, m_vPosition);
	fDistSquare = m3dGetVectorLengthSquared3(vDist);
	fDist = sqrtf(fDistSquare);

	memcpy(vIntensity, m_vIntensity, sizeof(M3DVector3f));
	m3dScaleVector3(vIntensity,
		1 / (m_vAttenuation[0] + m_vAttenuation[1] * fDist + m_vAttenuation[2] * fDistSquare));
}