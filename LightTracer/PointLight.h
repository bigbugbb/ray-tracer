#pragma once
#include "light.h"

class CPointLight : public CLight
{
public:
	CPointLight(void);
	virtual ~CPointLight(void);

	virtual void GetDirection(M3DVector3f vDirection, M3DVector3f vHitPoint);
	virtual void CalculateIntensity(M3DVector3f vIntensity, M3DVector3f vHitPoint = NULL);
};

