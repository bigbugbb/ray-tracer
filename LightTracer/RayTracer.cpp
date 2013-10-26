#include "stdafx.h"
#include <assert.h>
#include "Parser.h"
#include "RayTracer.h"
#include "Sphere.h"
#include "Plane.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "AmbientLight.h"

#include <fstream>

const GLfloat c_fMaxT = 100000;
const GLfloat c_fEpsilon = 0.001;

CRayTracer* const g_pRayTracer = CRayTracer::GetInstance();

#define clip(x, min, max)		((x) = (x) < (min) ? (min) : (x) > (max) ? (max) : (x))

inline void ClipVector3f(M3DVector3f vector, GLfloat fMin, GLfloat fMax)
{
	vector[0] = clip(vector[0], fMin, fMax);
	vector[1] = clip(vector[1], fMin, fMax);
	vector[2] = clip(vector[2], fMin, fMax);
}

inline void UpdateColor(M3DVector3f vColor, M3DVector3f vMaterial, M3DVector3f vIntensity)
{
	vColor[0] += vMaterial[0] * vIntensity[0];
	vColor[1] += vMaterial[1] * vIntensity[1];
	vColor[2] += vMaterial[2] * vIntensity[2];

	ClipVector3f(vColor, 0, 1);
}

inline void GetReflectedVector(M3DVector3f vReflected, M3DVector3f vSrcDir, M3DVector3f vNormal)
{	
	// r = d - 2 * (d dot n) * n
	GLfloat fTemp = 2 * m3dDotProduct3(vSrcDir, vNormal); // 2 * (d dot n)
	M3DVector3f vTemp;
	m3dCopyVector3(vTemp, vNormal);
	m3dScaleVector3(vTemp, fTemp); // 2 * (d dot n) * n
	m3dSubtractVectors3(vReflected, vSrcDir, vTemp); // d - 2 * (d dot n) * n
}

CRayTracer* CRayTracer::GetInstance()
{
	static CRayTracer s_RayTracer;

	return &s_RayTracer;
}

CRayTracer::CRayTracer(void)
{
	SetResolution(512, 512);

	memset(m_vBackground, 0, sizeof(M3DVector3f));
}


CRayTracer::~CRayTracer(void)
{
	Destroy();
}

void CRayTracer::GetResolution(GLint& nWidth, GLint& nHeight)
{
	nWidth  = m_nWidth;
	nHeight = m_nHeight;
}

void CRayTracer::SetBackground(M3DVector3f vBackground)
{
	m3dCopyVector3(m_vBackground, vBackground);
}

void CRayTracer::SetResolution(GLint nWidth, GLint nHeight)
{
	m_nWidth  = nWidth;
	m_nHeight = nHeight;
}

void CRayTracer::SetOutputFileName(const string& strOptFileName)
{
	m_strOptFileName = strOptFileName;
}

void CRayTracer::SetRecurDepth(GLint nRecurDepth)
{
	m_nRecurDepth = nRecurDepth;
}

void CRayTracer::AddLight(CLight* pLight)
{
	m_vecLights.push_back(pLight);
}

void CRayTracer::AddPrimitive(CPrimitive* pPrimitive)
{
	m_vecPrimitives.push_back(pPrimitive);
}

int CRayTracer::Load(const string& strTxt)
{
	CParser parser(strTxt.c_str());

	parser.Parse();

	return 0;
}

void CRayTracer::SetCamera(M3DVector3f vCamPos, M3DVector3f vCamTowards)
{
	m3dCopyVector3(m_vCamPos, vCamPos);
	m3dCopyVector3(m_vCamTowards, vCamTowards);
}

int CRayTracer::Save()
{
	BOOL Screenshot(const string& strFilePath);

	BOOL bSuccess = Screenshot(m_strOptFileName);
	fprintf(stdout, bSuccess ? (m_strOptFileName + " has been successfully saved.\n").c_str()
		: ("Fail to save " + m_strOptFileName + "!\n").c_str());

	return 0;
}

BOOL Screenshot(const string& strFilePath)
{
	BOOL WriteBMP(const string& strFileName, BYTE* pData, int nWidth, int nHeight);

	GLenum eBufferType;
	GLbyte* pBits = 0;
	ULONG ulImageSize;
	GLint nViewport[4];

	// get viewport size
	glGetIntegerv(GL_VIEWPORT, nViewport);
	ulImageSize = nViewport[2] * nViewport[3] * 3;

	pBits = (GLbyte*) new BYTE[ulImageSize];
	if (!pBits) {
		return FALSE;
	}

	// read data from color buffer
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	glGetIntegerv(GL_READ_BUFFER, (GLint*)&eBufferType);
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, nViewport[2], nViewport[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, pBits);
	glReadBuffer(eBufferType);

	return WriteBMP(strFilePath.c_str(), (BYTE*)pBits, nViewport[2], nViewport[3]);
}


BOOL WriteBMP(const string& strFileName, BYTE* pData, int nWidth, int nHeight)
{
	std::ofstream outfile;

	assert(pData);

	/** 创建位图文件信息和位图文件头结构 */
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bitmapInfoHeader;

	// open the file, if the file does not exist, create it.
	outfile.open(strFileName, std::ios::out | std::ios::binary);
	if (!outfile.is_open()) {
		//std::cerr << "Unable to open file " << filename << std::endl;
		return FALSE;
	}

	/** 填充BITMAPFILEHEADER */
	header.bfType      = 0x4D42;
	header.bfSize      = nWidth * nHeight * 3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	/** 写入位图文件头信息 */
	outfile.write((char*)&header, sizeof(BITMAPFILEHEADER));

	/** 填充BITMAPINFOHEADER */
	bitmapInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth         = nWidth;
	bitmapInfoHeader.biHeight        = nHeight;
	bitmapInfoHeader.biPlanes        = 1;
	bitmapInfoHeader.biBitCount      = 24;
	bitmapInfoHeader.biCompression   = BI_RGB; // BI_RLE4 BI_RLE8
	bitmapInfoHeader.biSizeImage     = nWidth * nHeight * 3; // 当压缩类型为BI_RGB是也可以设置为0
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed       = 0;
	bitmapInfoHeader.biClrImportant  = 0;
	/** 写入位图文件信息 */
	outfile.write((char*)&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

	/** 将指针移到数据开始位置 */
	outfile.seekp(header.bfOffBits, std::ios::beg);

	/** 写入图像数据 */
	outfile.write((char*) pData, bitmapInfoHeader.biSizeImage);

	outfile.close();
	return TRUE;
}

int CRayTracer::Config(int)
{
	return 0;
}

int CRayTracer::Prepare()
{
	M3DVector3f vTemp;

	// assume d = 1
	m_fFovHorzParam = tanf(M3D_PI / 3);
	m_fFovVertParam = tanf(M3D_PI / 3 / ((float) m_nWidth / m_nHeight));
	// calculate camera vectors
	M3DVector3f vUp = { 0, 1, 0 };	
	m3dCrossProduct3(m_vCamRight, m_vCamTowards, vUp);
	m3dCrossProduct3(m_vCamUp, m_vCamRight, m_vCamTowards);
	m3dNormalizeVector3(m_vCamTowards);
	m3dNormalizeVector3(m_vCamRight);
	m3dNormalizeVector3(m_vCamUp);
	// calculate camaera horizontal vector
	m3dAddVectors3(m_vLeft, m_vCamPos, m_vCamTowards); // p0 + d * towards
	m3dCopyVector3(vTemp, m_vCamRight);
	m3dScaleVector3(vTemp, m_fFovHorzParam); // d * tan(pi/3) * right
	m3dSubtractVectors3(m_vLeft, m_vLeft, vTemp); // p0 + d * towards - d * tan(pi/3) * right
	// calculate camaera vertical vector
	m3dAddVectors3(m_vBottom, m_vCamPos, m_vCamTowards); // p0 + d * towards
	m3dCopyVector3(vTemp, m_vCamUp);
	m3dScaleVector3(vTemp, m_fFovVertParam); // d * tan(pi/3) * up
	m3dSubtractVectors3(m_vBottom, m_vBottom, vTemp); // p0 + d * towards - d * tan(pi/3) * right

	return 0;
}

int CRayTracer::Render()
{		
	GLint w = m_nWidth >> 1;
	GLint h = m_nHeight >> 1;	
	Ray vray;

	// for each pixel
	for (GLint j = h; j > -h; --j) {
		for (GLint i = -w; i < w; ++i) {
			M3DVector3f vColor, vRayDir;
			// init the color to background
			m3dCopyVector3(vColor, m_vBackground);
			// generate the ray passing through this pixel			
			CreateViewingRay(i, j, vray);			
			// trace each ray and update the pixel color recursively			
			TraceRay(vColor, vray, c_fEpsilon, c_fMaxT, 0);
			// draw each pixel with the calculated color
			glBegin(GL_POINTS);
				glColor3fv(vColor);
				glVertex3f((float) i / w, (float) j / h, 0);				
			glEnd();
		}
	}
	glFlush();

	return 0;
}

int CRayTracer::Destroy()
{
	for (vector<CPrimitive*>::iterator iter = m_vecPrimitives.begin();
			iter != m_vecPrimitives.end(); ++iter) {
		CPrimitive* pPrimitive = *iter;
		delete pPrimitive;
	}
	m_vecPrimitives.clear();

	for (vector<CLight*>::iterator iter = m_vecLights.begin();
			iter != m_vecLights.end(); ++iter) {
		CLight* pLight = *iter;
		delete pLight;
	}
	m_vecLights.clear();

	return 0;
}

void CRayTracer::CreateViewingRay(GLint i, GLint j, Ray& rRay)
{
	rRay.eType = VIEWING_RAY;
	m3dCopyVector3(rRay.vSrc, m_vCamPos);
	// calculate the horizontal postition from i
	M3DVector3f vX;
	m3dCopyVector3(vX, m_vCamRight);
	m3dScaleVector3(vX, ((GLfloat) i / m_nWidth + 0.5f) * 2 * m_fFovHorzParam); // (i/width + 0.5) * 2 * d * tan(pi/3) * right
	m3dAddVectors3(vX, m_vLeft, vX);
	m3dSubtractVectors3(vX, vX, m_vCamPos);
	// calculate the vertical postition from j
	M3DVector3f vY;
	m3dCopyVector3(vY, m_vCamUp);
	m3dScaleVector3(vY, ((GLfloat) j / m_nHeight + 0.5f) * 2 * m_fFovVertParam); // (j/height + 0.5) * 2 * d * tan(pi/3/aspect) * up
	m3dAddVectors3(vY, m_vBottom, vY);
	m3dSubtractVectors3(vY, vY, m_vCamPos);
	// calculate the other point of the ray by adding vX and vY
	m3dAddVectors3(rRay.vDir, vX, vY);
	// vRayDir[2] /= 2;
	// get the direction of the current ray
	m3dNormalizeVector3(rRay.vDir);
}

inline void CRayTracer::LookForNearestIntersection(GLfloat* pfTargetT, CPrimitive** ppNearest, 
												   Ray& rRay, GLfloat fMinT, GLfloat fMaxT)
{
	assert(pfTargetT && ppNearest);
	*pfTargetT = fMaxT;

	// trace the ray and try to get the nearest intersecting primitive
	for (GLint i = 0; i < m_vecPrimitives.size(); ++i) {
		CPrimitive* pPrimitive = m_vecPrimitives[i];				
		// check for intersection
		if (pPrimitive->Intersect(rRay.vSrc, rRay.vDir, fMinT, fMaxT)) {
			// look for the nearest intersecting primitive
			const vector<GLfloat>& vecHitNote = pPrimitive->GetHitNote();
			for (int j = 0; j < vecHitNote.size(); ++j) {
				if (*pfTargetT > vecHitNote[j] && vecHitNote[j] > fMinT) {
					*pfTargetT = vecHitNote[j];
					*ppNearest = pPrimitive;
				}
			}						
		}
		pPrimitive->ClearHitNote(); // put here for safety
	}
}

inline void CRayTracer::TraceRay(M3DVector3f vColor, Ray& rRay, 
								 GLfloat fMinT, GLfloat fMaxT, GLint nCurDepth)
{
	M3DVector3f vHitPoint;
	GLfloat     fNearestT = fMaxT;
	CPrimitive* pNearestPrimitive = NULL;

	// if current recursive depth > max allowed recursive depth, just quit
	if (nCurDepth++ >= m_nRecurDepth) {
		return;
	}

	// trace the ray and try to get the nearest intersecting primitive
	LookForNearestIntersection(&fNearestT, &pNearestPrimitive, rRay, fMinT, fMaxT);

	// if found, try to estimate the color of this point
	if (pNearestPrimitive) {					
		// first get the hit point from fNearestT
		m3dCopyVector3(vHitPoint, rRay.vDir);
		m3dScaleVector3(vHitPoint, fNearestT);
		m3dAddVectors3(vHitPoint, rRay.vSrc, vHitPoint);
		// then estimate the color at the hit point
		// 1. consider the local illumination		
		EstimateLocalIllumination(vColor, rRay.eType, vHitPoint, fMinT, fMaxT, pNearestPrimitive);		
		// 2. consider the global illumination
		EstimateGlobalIllumination(vColor, vHitPoint, rRay.vDir, fMinT, fMaxT, pNearestPrimitive, nCurDepth);
		// 3. clip the color vector to make its elements between 0 and 1
		ClipVector3f(vColor, 0, 1);	
	}
}

void CRayTracer::EstimateLocalIllumination(M3DVector3f vColor, RayType eType, M3DVector3f vHitPoint,
										   GLfloat fMinT, GLfloat fMaxT, CPrimitive* pPrimitive)
{
	assert(pPrimitive != NULL);
	M3DVector3f vMaterial;
	M3DVector3f vIntensity;
	M3DVector3f vViewDir;
	M3DVector3f vNormal;
	M3DVector3f vLightDir;
	M3DVector3f vShadowRayDir;

	// local illumination
	for (int i = 0; i < m_vecLights.size(); ++i) {
		CLight* pLight = m_vecLights[i];
		BOOL bInShadow = FALSE;		
		M3DVector3f vDiffColor = { 0, 0, 0 };
		M3DVector3f vSpecColor = { 0, 0, 0 };

		// get light properties
		pLight->GetDirection(vLightDir, vHitPoint);
		pLight->CalculateIntensity(vIntensity, vHitPoint);
		ClipVector3f(vIntensity, 0, 1);
		// get normal and light vector at the hit point
		pPrimitive->GetNormal(vNormal, vHitPoint);
		m3dScaleVector3(vLightDir, -1); // from the hit point to the light	
		
		// create the shadow ray
		m3dCopyVector3(vShadowRayDir, vLightDir);
		// check if a primitive is occluding the shadow ray
		GLfloat fTargetT = fMaxT;
		CPrimitive* pNearestObstacle = NULL;
		// trace the ray and try to get the nearest intersecting primitive
		Ray sray(SHADOW_RAY, vHitPoint, vShadowRayDir);
		LookForNearestIntersection(&fTargetT, &pNearestObstacle, sray, fMinT, fMaxT);
		// if this obstacle is between the hit point and the light, then the hit point is in Shadow
		if (pNearestObstacle) {
			M3DVector3f vLightPos;
			M3DVector3f vLightDist;
			M3DVector3f vObstacleDist;
			// calculate light dist between hit point
			pLight->GetPosition(vLightPos);
			m3dSubtractVectors3(vLightDist, vLightPos, vHitPoint);
			// calculate obstacle dist between hit point
			m3dCopyVector3(vObstacleDist, vShadowRayDir);
			m3dScaleVector3(vObstacleDist, fTargetT);
			if (m3dGetVectorLengthSquared3(vLightDist) > m3dGetVectorLengthSquared3(vObstacleDist)) {
				bInShadow = TRUE;
			}
		}

		LightType eType = pLight->GetType();
		if (eType == LightType::AMBIENT_LIGHT) {
			// get ambient material color			
			pPrimitive->GetAmbientMaterial(vMaterial);
			// update color
			UpdateColor(vColor, vMaterial, vIntensity);
		} else if (eType == LightType::DIRECTIONAL_LIGHT || eType == LightType::POINT_LIGHT || eType == LightType::SPOT_LIGHT) {			
			// 1. calculate the diffuse color
			// get diffuse material color			
			pPrimitive->GetDiffuseMaterial(vMaterial);
			// estimate diffuse color
			EstimateDiffuse(vDiffColor, vMaterial, vNormal, vLightDir, vIntensity);
			// add the shadow effect
			m3dScaleVector3(vDiffColor, bInShadow ? 0.01 : 1);
			// add the diffuse color
			m3dAddVectors3(vColor, vColor, vDiffColor);
			
			// 2. calculate the specular color
			// get specular material color
			GLint nShininess = 16; // default value
			pPrimitive->GetSpecularMaterial(vMaterial, nShininess);
			// calculate view direction
			m3dSubtractVectors3(vViewDir, m_vCamPos, vHitPoint);
			m3dNormalizeVector3(vViewDir);
			// estimate specular color
			EstimateSpecular(vSpecColor, vMaterial, vViewDir, vNormal, vLightDir, vIntensity, nShininess);
			// add the shadow effect
			m3dScaleVector3(vSpecColor, bInShadow ? 0.01 : 1);
			// add the specular color
			m3dAddVectors3(vColor, vColor, vSpecColor);
		}
	}

}

void CRayTracer::EstimateGlobalIllumination(M3DVector3f vColor, M3DVector3f vHitPoint, M3DVector3f vSrcDir, 
											GLfloat fMinT, GLfloat fMaxT, CPrimitive* pPrimitive, GLint nCurDepth)
{
	M3DVector3f vNormal;
	M3DVector3f vReflected;
	M3DVector3f vReflectedColor = { 0, 0, 0 };

	// get the reflected direction
	pPrimitive->GetNormal(vNormal, vHitPoint);
	GetReflectedVector(vReflected, vSrcDir, vNormal);
	// trace ray in the reflected direction recursively
	Ray rflray(REFLECTED_RAY, vHitPoint, vReflected);
	TraceRay(vReflectedColor, rflray, fMinT, fMaxT, nCurDepth);
	// calculate the result color
	GLint nShininess; // just ignored
	M3DVector3f vSpecMaterial;
	pPrimitive->GetSpecularMaterial(vSpecMaterial, nShininess);
//	UpdateColor(vReflectedColor, vReflectedColor, vSpecMaterial);
	vReflectedColor[0] = vReflectedColor[0] * vSpecMaterial[0];
	vReflectedColor[1] = vReflectedColor[1] * vSpecMaterial[1];
	vReflectedColor[2] = vReflectedColor[2] * vSpecMaterial[2];
	m3dAddVectors3(vColor, vColor, vReflectedColor);
}

inline void CRayTracer::EstimateDiffuse(M3DVector3f vDiffColor, M3DVector3f vDiffMaterial, M3DVector3f vNormal,
										M3DVector3f vLightDir, M3DVector3f vIntensity)
{
	GLfloat fScale = m3dDotProduct3(vNormal, vLightDir);
	if (fScale <= 0) {
		m3dScaleVector3(vDiffColor, 0);
		return;
	}
	m3dScaleVector3(vDiffMaterial, fScale);
	UpdateColor(vDiffColor, vDiffMaterial, vIntensity);
}

inline void CRayTracer::EstimateSpecular(M3DVector3f vSpecColor, M3DVector3f vSpecMaterial, M3DVector3f vViewDir,
										 M3DVector3f vNormal, M3DVector3f vLightDir, M3DVector3f vIntensity, GLint nShininess)
{
	M3DVector3f vReflected;
	M3DVector3f vLightDirReversed;

	// get reflected light direction
	m3dCopyVector3(vLightDirReversed, vLightDir);
	m3dScaleVector3(vLightDirReversed, -1); // from light point to hit point
	GetReflectedVector(vReflected, vLightDirReversed, vNormal);	
	// calculate scale
	GLfloat fScale = m3dDotProduct3(vViewDir, vReflected);
	if (fScale <= 0) {
		m3dScaleVector3(vSpecColor, 0);
		return;		
	} else {
		fScale = powf(fScale, nShininess);
	}

	/*M3DVector3f vMiddle;
	m3dAddVectors3(vMiddle, vViewDir, vLightDir);
	m3dNormalizeVector3(vMiddle);
	GLfloat fScale = m3dDotProduct3(vMiddle, vNormal);
	if (fScale <= 0) {
		m3dScaleVector3(vSpecColor, 0);
		return;		
	} else {
		fScale = powf(fScale, nShininess);
	}*/
	// calculate specular color
	m3dScaleVector3(vSpecMaterial, fScale);
	UpdateColor(vSpecColor, vSpecMaterial, vIntensity);
}
