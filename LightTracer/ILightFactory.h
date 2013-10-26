#pragma once

class CLight;

enum LightType
{
	AMBIENT_LIGHT = 0,
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
};

struct ILightFactory
{
	virtual CLight* CreateLight(LightType eType, ...) = 0;
};
