#include "stdafx.h"
#include "LightFactory.h"
#include "RayTracer.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include <vector>
using namespace std;


CLightFactory::CLightFactory(void)
{
}


CLightFactory::~CLightFactory(void)
{
}

ILightFactory* CLightFactory::GetInstance()
{
	static CLightFactory s_LightFactory;

	return &s_LightFactory;
}

CLight* CLightFactory::CreateLight(LightType eType, ...)
{
	CLight* pLight = NULL;

	va_list arg_ptr;	
	va_start(arg_ptr, eType);

	if (eType == LightType::AMBIENT_LIGHT) {
		// get parameters	
		float r = *va_arg(arg_ptr, float*); 
		float g = *va_arg(arg_ptr, float*);
		float b = *va_arg(arg_ptr, float*);
		M3DVector3f vColor = { r, g, b };
		// create a sphere
		CAmbientLight* pAmbientLight = new CAmbientLight();		
		pAmbientLight->SetIntensity(vColor);
		// add the light to CRayTracer
		pLight = pAmbientLight;		
		g_pRayTracer->AddLight(pLight);
	} else if (eType == LightType::DIRECTIONAL_LIGHT) {
		// get parameters
		vector<float>& vecValues = *va_arg(arg_ptr, vector<float>*);				
		float r = *va_arg(arg_ptr, float*); 
		float g = *va_arg(arg_ptr, float*);
		float b = *va_arg(arg_ptr, float*);
		M3DVector3f vColor = { r, g, b };
		M3DVector3f vDirection = { vecValues[3], vecValues[4], vecValues[5] };
		// create a sphere
		CDirectionalLight* pDirectionalLight = new CDirectionalLight();
		pDirectionalLight->SetIntensity(vColor);
		pDirectionalLight->SetDirection(vDirection);
		// add the light to CRayTracer
		pLight = pDirectionalLight;
		g_pRayTracer->AddLight(pLight);
	} else if (eType == LightType::POINT_LIGHT) {
		// get parameters
		vector<float>& vecValues = *va_arg(arg_ptr, vector<float>*);				
		float r = *va_arg(arg_ptr, float*); 
		float g = *va_arg(arg_ptr, float*);
		float b = *va_arg(arg_ptr, float*);
		M3DVector3f vColor = { r, g, b };
		M3DVector3f vPosition  = { vecValues[0], vecValues[1], vecValues[2] };
		M3DVector3f vDirection = { vecValues[3], vecValues[4], vecValues[5] };
		//M3DVector3f vAttenuation = { 0, 0, 1 };
		// create a sphere
		CPointLight* pPointLight = new CPointLight();
		pPointLight->SetIntensity(vColor);
		pPointLight->SetPosition(vPosition);
		pPointLight->SetDirection(vDirection);
		//pPointLight->SetAttenuation(vAttenuation);
		// add the light to CRayTracer
		pLight = pPointLight;
		g_pRayTracer->AddLight(pLight);
	} else if (eType == LightType::SPOT_LIGHT) {
		// get parameters
		vector<float>& vecValues = *va_arg(arg_ptr, vector<float>*);				
		float r = *va_arg(arg_ptr, float*); 
		float g = *va_arg(arg_ptr, float*);
		float b = *va_arg(arg_ptr, float*);
		M3DVector3f vColor = { r, g, b };
		M3DVector3f vPosition  = { vecValues[0], vecValues[1], vecValues[2] };
		M3DVector3f vDirection = { vecValues[3], vecValues[4], vecValues[5] };
		//M3DVector3f vAttenuation = { 0, 0, 1 };
		// create a sphere
		CSpotLight* pSpotLight = new CSpotLight();
		pSpotLight->SetIntensity(vColor);
		pSpotLight->SetPosition(vPosition);
		pSpotLight->SetDirection(vDirection);
		//pSpotLight->SetAttenuation(vAttenuation);
		// add the light to CRayTracer
		pLight = pSpotLight;
		g_pRayTracer->AddLight(pLight);
	}

	va_end(arg_ptr);

	return pLight;
}