#include "stdafx.h"
#include "Light.h"


CLight::CLight(void)
{
}


CLight::~CLight(void)
{
}

void CLight::SetPosition(M3DVector3f vPosition)
{
	m3dCopyVector3(m_vPosition, vPosition);
}

void CLight::SetDirection(M3DVector3f vDirection)
{
	m3dCopyVector3(m_vDirection, vDirection);
	m3dNormalizeVector3(m_vDirection);
}

void CLight::SetAttenuation(M3DVector3f vAttenuation)
{
	m3dCopyVector3(m_vAttenuation, vAttenuation);
}

void CLight::SetIntensity(M3DVector3f vIntensity)
{
	extern void ClipVector3f(M3DVector3f vector, GLfloat fMin, GLfloat fMax);

	m3dCopyVector3(m_vIntensity, vIntensity);
	ClipVector3f(m_vIntensity, 0, 1);
}

void CLight::GetPosition(M3DVector3f vPosition)
{
	m3dCopyVector3(vPosition, m_vPosition);
}

void CLight::GetDirection(M3DVector3f vDirection, M3DVector3f vHitPoint)
{
	m3dCopyVector3(vDirection, m_vDirection);
}

void CLight::GetAttenuation(M3DVector3f vAttenuation)
{
	m3dCopyVector3(vAttenuation, m_vAttenuation);
}