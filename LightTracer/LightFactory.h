#pragma once
#include "ILightFactory.h"

class CLightFactory : public ILightFactory
{
public:
	static ILightFactory* GetInstance();
	virtual CLight* CreateLight(LightType eType, ...);

protected:
	CLightFactory(void);
	~CLightFactory(void);
};

