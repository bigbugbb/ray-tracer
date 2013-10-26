#pragma once

#include "Light.h"
#include "Primitive.h"

#include <list>
using std::list;

#include <string>
using std::string;

class CRayTracer
{
public:
	static CRayTracer* GetInstance();

	int Load(const string& strTxt);	
	int Save();
	int Config(int);
	int Prepare();
	int Render();	
	int Destroy();

	// get tracer properties
	void GetResolution(GLint& nWidth, GLint& nHeight);
	
	// set tracer properties
	void SetBackground(M3DVector3f vBackground);
	void SetResolution(GLint nWidth, GLint nHeight);
	void SetRecurDepth(GLint nRecurDepth);
	void SetCamera(M3DVector3f vCamPos, M3DVector3f vCamTowards);
	void SetOutputFileName(const string& strOptFileName);

	// add lights and primitives to the tracer
	void AddLight(CLight* pLight);
	void AddPrimitive(CPrimitive* pPrimitive);

protected:
	CRayTracer(void);
	~CRayTracer(void);

	enum RayType
	{
		VIEWING_RAY = 0,
		SHADOW_RAY,
		REFLECTED_RAY,
		REFRACTED_RAY
	};

	struct Ray
	{
		Ray() {}

		Ray(RayType eType, M3DVector3f vSrc, M3DVector3f vDir)
		{
			this->eType = eType;
			m3dCopyVector3(this->vSrc, vSrc);
			m3dCopyVector3(this->vDir, vDir);
		}

		Ray(const Ray& rRay)
		{
			this->eType = rRay.eType;
			m3dCopyVector3(this->vSrc, rRay.vSrc);
			m3dCopyVector3(this->vDir, rRay.vDir);
		}

		RayType		eType;
		M3DVector3f	vSrc;
		M3DVector3f	vDir;
	};

	void CreateViewingRay(GLint i, GLint j, Ray& rRay);
	void TraceRay(M3DVector3f vColor, Ray& rRay, GLfloat fMinT, GLfloat fMaxT, GLint nCurDepth);
	void LookForNearestIntersection(GLfloat* pfTargetT, CPrimitive** ppNearest, 
		Ray& rRay, GLfloat fMinT, GLfloat fMaxT);
	void EstimateLocalIllumination(M3DVector3f vColor, RayType eType, M3DVector3f vHitPoint, 
		GLfloat fMinT, GLfloat fMaxT, CPrimitive* pPrimitive);
	void EstimateGlobalIllumination(M3DVector3f vColor, M3DVector3f vHitPoint, M3DVector3f vSrcDir, 
		GLfloat fMinT, GLfloat fMaxT, CPrimitive* pPrimitive, GLint nCurDepth);
	void EstimateDiffuse(M3DVector3f vDiffColor, M3DVector3f vDiffMaterial, 
		M3DVector3f vNormal, M3DVector3f vLightDir, M3DVector3f vIntensity);
	void EstimateSpecular(M3DVector3f vSpecColor, M3DVector3f vSpecMaterial, M3DVector3f vViewDir,
		M3DVector3f vNormal, M3DVector3f vLightDir, M3DVector3f vIntensity, GLint nShininess);

	vector<CLight*>		m_vecLights;	 // all lights in the scene
	vector<CPrimitive*> m_vecPrimitives; // all primitives in the scene

	GLint				m_nWidth;		 // image width
	GLint				m_nHeight;		 // image height
	GLint				m_nRecurDepth;	 // recursion depth
	
	GLfloat				m_fFovHorzParam; // tanf(M3D_PI / 6)
	GLfloat				m_fFovVertParam; // tanf(M3D_PI / 6 / ((float) m_nWidth / m_nHeight));
	M3DVector3f			m_vCamPos;		 // camera position
	M3DVector3f			m_vCamTowards;	 // camera towards direction
	M3DVector3f			m_vCamRight;	 // camera right direction
	M3DVector3f			m_vCamUp;
	M3DVector3f			m_vBackground;	 // background color in r, g, b

	M3DVector3f			m_vLeft;
	M3DVector3f			m_vBottom;

	string				m_strOptFileName;
};

extern CRayTracer* const g_pRayTracer;
