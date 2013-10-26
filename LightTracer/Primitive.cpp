#include "stdafx.h"
#include "Primitive.h"


CPrimitive::CPrimitive(void)
{
}

CPrimitive::~CPrimitive(void)
{
}

void CPrimitive::SetAmbientMaterial(M3DVector3f vColor)
{
	m3dCopyVector3(m_vAmbient, vColor);
}

void CPrimitive::SetDiffuseMaterial(M3DVector3f vColor)
{
	m3dCopyVector3(m_vDiffuse, vColor);
}

void CPrimitive::SetSpecularMaterial(M3DVector3f vColor, GLint nShininess)
{
	m3dCopyVector3(m_vSpecular, vColor);
	m_nShininess = nShininess;
}

void CPrimitive::SetTransmissiveMaterial(M3DVector3f vColor, GLint nIOR)
{
	m3dCopyVector3(m_vTransmissive, vColor);
	m_nIOR = nIOR;
}

void CPrimitive::GetAmbientMaterial(M3DVector3f vColor)
{
	m3dCopyVector3(vColor, m_vAmbient);
}

void CPrimitive::GetDiffuseMaterial(M3DVector3f vColor)
{
	m3dCopyVector3(vColor, m_vDiffuse);
}

void CPrimitive::GetSpecularMaterial(M3DVector3f vColor, GLint& rnShininess)
{
	rnShininess = m_nShininess;
	m3dCopyVector3(vColor, m_vSpecular);
}

void CPrimitive::GetTransmissiveMaterial(M3DVector3f vColor, GLint& rnIOR)
{
	rnIOR = m_nIOR;
	m3dCopyVector3(vColor, m_vTransmissive);
}

const vector<GLfloat>& CPrimitive::GetHitNote()
{
	return m_vecHitNote;
}

void CPrimitive::ClearHitNote()
{
	m_vecHitNote.clear();
}

PrimitiveType CPrimitive::GetType()
{
	return m_eType;
}