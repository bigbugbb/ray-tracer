#include "stdafx.h"
#include "DirectionalLight.h"


CDirectionalLight::CDirectionalLight(void)
{
	m_eType = LightType::DIRECTIONAL_LIGHT;
}


CDirectionalLight::~CDirectionalLight(void)
{
}

void CDirectionalLight::CalculateIntensity(M3DVector3f vIntensity, M3DVector3f)
{
	m3dCopyVector3(vIntensity, m_vIntensity);
}