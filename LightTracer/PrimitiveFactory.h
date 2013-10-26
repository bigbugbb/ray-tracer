#pragma once
#include "IPrimitiveFactory.h"

class CPrimitiveFactory : public IPrimitiveFactory
{
public:
	static IPrimitiveFactory* GetInstance();

	// used to create each different primitive

	//if eType == SPHERE
	//the argument is (SPHERE,int nIndex,M3DVector4f vecfAM,M3DVector4f vecfDM,M3DVector4f vecfSM)
	//nIndex means the index in the vertex vectors

	//if eType == TRIANGLE
	//the argument is (SPHERE,M3DVector3d vecdIndex,M3DVector4f vecfAM,M3DVector4f vecfDM,M3DVector4f vecfSM)
	//vecdIndex means the index vetor in the vertex vectors
	
	//vecfAM means AmbientMaterial(r,g,b)
	//vecfDM means DiffuseMaterial(r,g,b)
	//vecfSM means SpecularMaterial(r,g,b,n)
	virtual CPrimitive* CreatePrimitive(PrimitiveType eType, ...);

protected:
	CPrimitiveFactory(void);
	~CPrimitiveFactory(void);
};

