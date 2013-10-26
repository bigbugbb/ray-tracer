#include "stdafx.h"
#include "SpotLight.h"


CSpotLight::CSpotLight(void)
{
	m_eType = LightType::SPOT_LIGHT;
}


CSpotLight::~CSpotLight(void)
{
}

void CSpotLight::GetDirection(M3DVector3f vDirection, M3DVector3f vHitPoint)
{
	m3dSubtractVectors3(vDirection, vHitPoint, m_vPosition);
	m3dNormalizeVector3(vDirection);
}

void CSpotLight::CalculateIntensity(M3DVector3f vIntensity, M3DVector3f vHitPoint)
{
	M3DVector3f vLight;

	m3dSubtractVectors3(vLight, vHitPoint, m_vPosition);
	m3dNormalizeVector3(vLight);	

	CPointLight::CalculateIntensity(vIntensity, vHitPoint);
	GLfloat fScale = m3dDotProduct3(vLight, m_vDirection);
	m3dScaleVector3(vIntensity, fScale);
}