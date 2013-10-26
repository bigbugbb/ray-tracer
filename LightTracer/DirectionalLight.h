#pragma once
#include "light.h"

class CDirectionalLight : public CLight
{
public:
	CDirectionalLight(void);
	virtual ~CDirectionalLight(void);

	virtual void CalculateIntensity(M3DVector3f vIntensity, M3DVector3f vHitPoint = NULL);
};

