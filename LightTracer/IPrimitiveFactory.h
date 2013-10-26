#pragma once

class CPrimitive;

enum PrimitiveType
{
	SPHERE = 0,
	TRIANGLE,
	PLANE,
};

struct IPrimitiveFactory
{
	virtual CPrimitive* CreatePrimitive(PrimitiveType eType, ...) = 0;
};

