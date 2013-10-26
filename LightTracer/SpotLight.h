#pragma once
#include "PointLight.h"

class CSpotLight : public CPointLight
{
public:
	CSpotLight(void);
	virtual ~CSpotLight(void);

	virtual void GetDirection(M3DVector3f vDirection, M3DVector3f vHitPoint);
	virtual void CalculateIntensity(M3DVector3f vIntensity, M3DVector3f vHitPoint = NULL);
};

