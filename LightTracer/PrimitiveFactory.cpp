#include "stdafx.h"
#include <stdarg.h>
#include <GLTools.h>
#include "PrimitiveFactory.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "RayTracer.h"

#include <vector>
using namespace std;


CPrimitiveFactory::CPrimitiveFactory(void)
{
}


CPrimitiveFactory::~CPrimitiveFactory(void)
{
}

IPrimitiveFactory* CPrimitiveFactory::GetInstance()
{
	static CPrimitiveFactory s_PrimitiveFactory;

	return &s_PrimitiveFactory;
}

//if eType == SPHERE
//the argument is (SPHERE,int nIndex,M3DVector4f vecfAM,M3DVector4f vecfDM,M3DVector4f vecfSM)
//nIndex means the index in the vertex vectors

//if eType == TRIANGLE
//the argument is (SPHERE,M3DVector3d vecdIndex,M3DVector4f vecfAM,M3DVector4f vecfDM,M3DVector4f vecfSM)
//vecdIndex means the index vetor in the vertex vectors
	
//vecfAM means AmbientMaterial(r,g,b)
//vecfDM means DiffuseMaterial(r,g,b)
//vecfSM means SpecularMaterial(r,g,b,n)

CPrimitive* CPrimitiveFactory::CreatePrimitive(PrimitiveType eType, ...)
{
	CPrimitive* pPrimitive = NULL;

	va_list arg_ptr;	
	va_start(arg_ptr, eType);

	if (eType == PrimitiveType::SPHERE) {
		// get parameters
		vector<vector<float>>& vvValues = *va_arg(arg_ptr, vector<vector<float>>*);		
		int i = va_arg(arg_ptr, int); 
		vector<float>& vValues = vvValues[i]; // ignore the boundary check
		M3DVector4f& vAM = *va_arg(arg_ptr, M3DVector4f*);
		M3DVector4f& vDM = *va_arg(arg_ptr, M3DVector4f*);
		M3DVector4f& vSM = *va_arg(arg_ptr, M3DVector4f*);
		// create a sphere
		CSphere* pSphere = new CSphere();
		pSphere->SetCenter(vValues[0], vValues[1], vValues[2]);
		pSphere->SetRadius(sqrtf(vValues[3] * vValues[3] + vValues[4] * vValues[4] + vValues[5] * vValues[5]));	
		pSphere->SetAmbientMaterial(vAM);
		pSphere->SetDiffuseMaterial(vDM);
		pSphere->SetSpecularMaterial(vSM, (int)vSM[3]);
		pPrimitive = pSphere;
		// add the primitive to CRayTracer
		g_pRayTracer->AddPrimitive(pSphere);
	} else if (eType == PrimitiveType::TRIANGLE) {
		// get parameters
		vector<vector<float>>& vvValues = *va_arg(arg_ptr, vector<vector<float>>*);
		int i1 = va_arg(arg_ptr, int);
		int i2 = va_arg(arg_ptr, int);
		int i3 = va_arg(arg_ptr, int);
		vector<float>& vValue1 = vvValues[i1];
		vector<float>& vValue2 = vvValues[i2];
		vector<float>& vValue3 = vvValues[i3];
		M3DVector4f& vAM = *va_arg(arg_ptr, M3DVector4f*);
		M3DVector4f& vDM = *va_arg(arg_ptr, M3DVector4f*);
		M3DVector4f& vSM = *va_arg(arg_ptr, M3DVector4f*);
		// create a triangle
		CTriangle* pTriangle = new CTriangle();		
		M3DVector3f vPoint1 = { vValue1[0], vValue1[1], vValue1[2] };	
		M3DVector3f vPoint2 = { vValue2[0], vValue2[1], vValue2[2] };
		M3DVector3f vPoint3 = { vValue3[0], vValue3[1], vValue3[2] };
		pTriangle->SetPosition(vPoint1, vPoint2, vPoint3);
		M3DVector3f vNormal = { vValue1[3], vValue1[4], vValue1[5] };
		pTriangle->SetNormal(vNormal);
		pTriangle->SetAmbientMaterial(vAM);
		pTriangle->SetDiffuseMaterial(vDM);		
		pTriangle->SetSpecularMaterial(vSM, (int)vSM[3]);
		pPrimitive = pTriangle;
		g_pRayTracer->AddPrimitive(pPrimitive);
	} else if (eType == PrimitiveType::PLANE) {
		// get parameters
		vector<vector<float>>& vvValues = *va_arg(arg_ptr, vector<vector<float>>*);		
		int i = va_arg(arg_ptr, int); 
		vector<float>& vValues = vvValues[i]; // ignore the boundary check
		M3DVector4f& vAM = *va_arg(arg_ptr, M3DVector4f*);
		M3DVector4f& vDM = *va_arg(arg_ptr, M3DVector4f*);
		M3DVector4f& vSM = *va_arg(arg_ptr, M3DVector4f*);
		// create a plane
		CPlane* pPlane = new CPlane();
		M3DVector3f vPosition = { vValues[0], vValues[1], vValues[2] };
		pPlane->SetPosition(vPosition);
		M3DVector3f vNormal = { vValues[3], vValues[4], vValues[5] };
		pPlane->SetNormal(vNormal);
		pPlane->SetAmbientMaterial(vAM);
		pPlane->SetDiffuseMaterial(vDM);		
		pPlane->SetSpecularMaterial(vSM, (int)vSM[3]);
		pPrimitive = pPlane;
		g_pRayTracer->AddPrimitive(pPlane);
	}

	va_end(arg_ptr);

	return pPrimitive;
}
