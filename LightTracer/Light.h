#pragma once

#include <GLTools.h>
#include "ILightFactory.h"

class CLight
{
public:
	CLight(void);
	virtual ~CLight(void);

	// get light type
	LightType GetType() { return m_eType; }

	// set properties	
	void SetPosition(M3DVector3f vPosition);
	void SetDirection(M3DVector3f vDirection);
	void SetAttenuation(M3DVector3f vAttenuation);
	void SetIntensity(M3DVector3f vIntensity);

	// get properties
	void GetPosition(M3DVector3f vPosition);	
	void GetAttenuation(M3DVector3f vAttenuation);
	virtual void GetDirection(M3DVector3f vDirection, M3DVector3f vHitPoint = NULL);
	// used to calculate the current light intensity of the specific hit point
	virtual void CalculateIntensity(M3DVector3f vIntensity, M3DVector3f vHitPoint = NULL) = 0;

protected:
	LightType		m_eType;

	// parameters for describing the light
	// position:    x, y, z
	// direction:   dx, dy, dz 
	// attenuation: a, b, c
	// intensity:   r, g, b
	M3DVector3f		m_vPosition;
	M3DVector3f		m_vDirection;
	M3DVector3f		m_vAttenuation;
	M3DVector3f		m_vIntensity;
};

