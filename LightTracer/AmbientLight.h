#pragma once
#include "light.h"

class CAmbientLight : public CLight
{
public:
	CAmbientLight(void);
	virtual ~CAmbientLight(void);

	virtual void CalculateIntensity(M3DVector3f vIntensity, M3DVector3f vHitPoint = NULL);
};

