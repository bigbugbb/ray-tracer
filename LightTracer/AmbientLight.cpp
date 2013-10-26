#include "stdafx.h"
#include "AmbientLight.h"


CAmbientLight::CAmbientLight(void)
{
	m_eType = LightType::AMBIENT_LIGHT;
}


CAmbientLight::~CAmbientLight(void)
{
}

void CAmbientLight::CalculateIntensity(M3DVector3f vIntensity, M3DVector3f)
{
	memcpy(vIntensity, m_vIntensity, sizeof(M3DVector3f));
}
