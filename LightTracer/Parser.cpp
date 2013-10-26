#include "stdafx.h"
#include <GLTools.h>
#include "Parser.h"
#include "RayTracer.h"
#include "LightFactory.h"
#include "PrimitiveFactory.h"


CParser::CParser(const char* chPath)
{
	m_strPath = chPath;
	
	m_vecVertex.clear();
	m_vecBkg.clear();
	
	memset(&m_struMaterial,NULL,sizeof(struMaterial));
	memset(&m_struLight,NULL,sizeof(struLight));
	memset(&m_strImgRes,NULL,sizeof(struImgRes));
	
	m_fPlane = 0.0f;
	m_fCamera = 0.0f;
	m_nRdepth = 0;
}

CParser::~CParser()
{
	
}

void CParser::Parse()
{
	FirstPass();
	SecondPass();
}

void CParser::FirstPass()
{
	FILE* pf = NULL;
	char cstr[256] = {0};
	string strBuf;
	strBuf.clear();

	fopen_s(&pf, m_strPath.c_str(),"r");
	if ( pf )
	{
		while(fgets(cstr,255,pf))
		{
			GetLineType(cstr);
			if(!strcmp(m_chLineType,"vv"))
			{
				ParseVV(cstr);
			}
		}
		fclose(pf);
	}
}

void CParser::SecondPass()
{
	char cstr[256] = {0};
	string strBuf;
	strBuf.clear();

	FILE* fp;
	fopen_s(&fp, m_strPath.c_str(),"r");
	if (fp)
	{
		while (fgets(cstr,255,fp))
		{
			GetLineType(cstr);
			ParseOneLine(cstr);
		}
		fclose(fp);
	}
}


void CParser::GetVertex(vector<vecfloat>& vv)
{
	vv = m_vecVertex;
}

void CParser::GetMaterial(struMaterial& mt)
{
	mt = m_struMaterial;
}

float CParser::GetPlane()
{
	return m_fPlane;
}

void CParser::GetLight(struLight& lt)
{
	lt = m_struLight;
}

float CParser::GetCamera()
{
	return m_fCamera;
}

void CParser::GetImgRes(struImgRes& ir)
{
	ir = m_strImgRes;
}

const char* CParser::GetOutputImg()
{
	return m_strOutputImg.c_str();
}

void CParser::GetBack(vecfloat& vecBkg)
{
	vecBkg = m_vecBkg;
}

int CParser::GetRDepth()
{
	return m_nRdepth;
}

void CParser::GetLineType(char* cstr)
{
	memset(m_chLineType,NULL,10);
	sscanf_s(cstr, "%[^ ]", m_chLineType,_countof(m_chLineType));
}

void CParser::ParseOneLine(char* chbuf)
{
	if( !strcmp(m_chLineType,"am"))
	{
		ParseAM(chbuf);
	}
	if( !strcmp(m_chLineType,"dm"))
	{
		ParseDM(chbuf);
	}		
	if( !strcmp(m_chLineType,"sm"))
	{
		ParseSM(chbuf);
	}
	if( !strcmp(m_chLineType,"tm"))
	{
		ParseTM(chbuf);
	}	
	if( !strcmp(m_chLineType,"ss"))
	{
		ParseSS(chbuf);
	}
	if( !strcmp(m_chLineType,"ts"))
	{
		ParseTS(chbuf);
	}
	if( !strcmp(m_chLineType,"ps"))
	{
		ParsePS(chbuf);
	}
	if( !strcmp(m_chLineType,"pl"))
	{
		ParsePL(chbuf);
	}
	if( !strcmp(m_chLineType,"dl"))
	{
		ParseDL(chbuf);
	}	
	if( !strcmp(m_chLineType,"sl"))
	{
		ParseSL(chbuf);
	}	
	if( !strcmp(m_chLineType,"al"))
	{
		ParseAL(chbuf);
	}
	if( !strcmp(m_chLineType,"cc"))
	{
		ParseCC(chbuf);
	}
	if( !strcmp(m_chLineType,"ir"))
	{
		ParseIR(chbuf);
	}
	if( !strcmp(m_chLineType,"output"))
	{
		ParseOP(chbuf);
	}
	if( !strcmp(m_chLineType,"back"))
	{
		ParseBK(chbuf);
	}
	if( !strcmp(m_chLineType,"rdepth"))
	{
		ParseRD(chbuf);
	}
}


void CParser::ParseVV(char* chbuf)
{
	float x = 0;
	float y = 0;
	float z = 0;
	float dx = 0;
	float dy = 0;
	float dz = 0;
	sscanf_s(chbuf,"%s %f %f %f %f %f %f",m_chLineType,10,&x,&y,&z,&dx,&dy,&dz);
	vecfloat vf;
	vf.clear();
	vf.push_back(x);
	vf.push_back(y);
	vf.push_back(z);
	vf.push_back(dx);
	vf.push_back(dy);
	vf.push_back(dz);
	m_vecVertex.push_back(vf);
}

void CParser::ParseAM(char* chbuf)
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	sscanf_s(chbuf,"%s %f %f %f",m_chLineType,10,&r,&g,&b);
	m_struMaterial.vecAmbientMaterial.clear();
	m_struMaterial.vecAmbientMaterial.push_back(r);
	m_struMaterial.vecAmbientMaterial.push_back(g);
	m_struMaterial.vecAmbientMaterial.push_back(b);
}

void CParser::ParseDM(char* chbuf)
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	sscanf_s(chbuf,"%s %f %f %f",m_chLineType,10,&r,&g,&b);
	m_struMaterial.vecDiffuseMaterial.clear();
	m_struMaterial.vecDiffuseMaterial.push_back(r);
	m_struMaterial.vecDiffuseMaterial.push_back(g);
	m_struMaterial.vecDiffuseMaterial.push_back(b);
}

void CParser::ParseSM(char* chbuf)
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float n = 0.0f;
	sscanf_s(chbuf,"%s %f %f %f %f",m_chLineType,10,&r,&g,&b,&n);
	m_struMaterial.vecSpecularMaterial.push_back(r);
	m_struMaterial.vecSpecularMaterial.push_back(g);
	m_struMaterial.vecSpecularMaterial.push_back(b);
	m_struMaterial.vecSpecularMaterial.push_back(n);
}

void CParser::ParseTM(char* chbuf)
{
	/*float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float n = 0.0f;
	sscanf_s(chbuf,"%s %f %f %f %f",m_chLineType,10,&r,&g,&b,&n);
	m_struMaterial.vecTransmissiveMaterial.push_back(r);
	m_struMaterial.vecTransmissiveMaterial.push_back(g);
	m_struMaterial.vecTransmissiveMaterial.push_back(b);
	m_struMaterial.vecTransmissiveMaterial.push_back(n);*/
}

void CParser::ParseSS(char* chbuf)
{
	int nIndex = 0;
	sscanf_s(chbuf,"%s %d",m_chLineType,10,&nIndex);
	M3DVector4f vecfAM;
	vecfAM[0] = m_struMaterial.vecAmbientMaterial[0];
	vecfAM[1] = m_struMaterial.vecAmbientMaterial[1];
	vecfAM[2] = m_struMaterial.vecAmbientMaterial[2];
	M3DVector4f vecfDM;
	vecfDM[0] = m_struMaterial.vecDiffuseMaterial[0];
	vecfDM[1] = m_struMaterial.vecDiffuseMaterial[1];
	vecfDM[2] = m_struMaterial.vecDiffuseMaterial[2];
	M3DVector4f vecfSM;
	vecfSM[0] = m_struMaterial.vecSpecularMaterial[0];
	vecfSM[1] = m_struMaterial.vecSpecularMaterial[1];
	vecfSM[2] = m_struMaterial.vecSpecularMaterial[2];
	vecfSM[3] = m_struMaterial.vecSpecularMaterial[3];

	CPrimitiveFactory::GetInstance()->CreatePrimitive(SPHERE,&m_vecVertex,nIndex,&vecfAM,&vecfDM,&vecfSM);
}

void CParser::ParseTS(char* chbuf)
{
	int nIndex1 = 0;
	int nIndex2 = 0;
	int nIndex3 = 0;
	sscanf_s(chbuf,"%s %d %d %d",m_chLineType,10,&nIndex1,&nIndex2,&nIndex3);
	M3DVector4f vecfAM;
	vecfAM[0] = m_struMaterial.vecAmbientMaterial[0];
	vecfAM[1] = m_struMaterial.vecAmbientMaterial[1];
	vecfAM[2] = m_struMaterial.vecAmbientMaterial[2];
	M3DVector4f vecfDM;
	vecfDM[0] = m_struMaterial.vecDiffuseMaterial[0];
	vecfDM[1] = m_struMaterial.vecDiffuseMaterial[1];
	vecfDM[2] = m_struMaterial.vecDiffuseMaterial[2];
	M3DVector4f vecfSM;
	vecfSM[0] = m_struMaterial.vecSpecularMaterial[0];
	vecfSM[1] = m_struMaterial.vecSpecularMaterial[1];
	vecfSM[2] = m_struMaterial.vecSpecularMaterial[2];
	vecfSM[3] = m_struMaterial.vecSpecularMaterial[3];

	CPrimitiveFactory::GetInstance()->CreatePrimitive(TRIANGLE,&m_vecVertex,nIndex1,nIndex2,nIndex3,&vecfAM,&vecfDM,&vecfSM);
}

void CParser::ParsePS(char* chbuf)
{
	int nIndex = 0;
	sscanf_s(chbuf,"%s %d",m_chLineType,10,&nIndex);
	M3DVector4f vecfAM;
	vecfAM[0] = m_struMaterial.vecAmbientMaterial[0];
	vecfAM[1] = m_struMaterial.vecAmbientMaterial[1];
	vecfAM[2] = m_struMaterial.vecAmbientMaterial[2];
	M3DVector4f vecfDM;
	vecfDM[0] = m_struMaterial.vecDiffuseMaterial[0];
	vecfDM[1] = m_struMaterial.vecDiffuseMaterial[1];
	vecfDM[2] = m_struMaterial.vecDiffuseMaterial[2];
	M3DVector4f vecfSM;
	vecfSM[0] = m_struMaterial.vecSpecularMaterial[0];
	vecfSM[1] = m_struMaterial.vecSpecularMaterial[1];
	vecfSM[2] = m_struMaterial.vecSpecularMaterial[2];
	vecfSM[3] = m_struMaterial.vecSpecularMaterial[3];

	CPrimitiveFactory::GetInstance()->CreatePrimitive(PLANE,&m_vecVertex,nIndex,&vecfAM,&vecfDM,&vecfSM);
}

void CParser::ParsePL(char* chbuf)
{
	int i;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	sscanf_s(chbuf,"%s %d %f %f %f",m_chLineType,10,&i,&r,&g,&b);

	CLightFactory::GetInstance()->CreateLight(POINT_LIGHT, &m_vecVertex[i], &r, &g, &b);
}

void CParser::ParseDL(char* chbuf)
{
	int i;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	sscanf_s(chbuf,"%s %d %f %f %f",m_chLineType,10,&i,&r,&g,&b);

	CLightFactory::GetInstance()->CreateLight(DIRECTIONAL_LIGHT, &m_vecVertex[i], &r, &g, &b);
}

void CParser::ParseSL(char* chbuf)
{
	int i;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	sscanf_s(chbuf,"%s %d %f %f %f",m_chLineType,10,&i,&r,&g,&b);

	CLightFactory::GetInstance()->CreateLight(SPOT_LIGHT, &m_vecVertex[i], &r, &g, &b);
}


void CParser::ParseAL(char* chbuf)
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	sscanf_s(chbuf,"%s %f %f %f",m_chLineType,10,&r,&g,&b);

	CLightFactory::GetInstance()->CreateLight(AMBIENT_LIGHT, &r, &g, &b);
}

void CParser::ParseCC(char* chbuf)
{
	int i;
	sscanf_s(chbuf,"%s %d",m_chLineType,10,&i);
	vector<float> vecValue = m_vecVertex[i];
	M3DVector3f vCamPos     = { vecValue[0], vecValue[1], vecValue[2] };
	M3DVector3f vCamTowards = { vecValue[3], vecValue[4], vecValue[5] };

	g_pRayTracer->SetCamera(vCamPos, vCamTowards);
}

void CParser::ParseIR(char* chbuf)
{
	sscanf_s(chbuf,"%s %d %d",m_chLineType,10,&m_strImgRes.nWidth, &m_strImgRes.nHeight);

	g_pRayTracer->SetResolution(m_strImgRes.nWidth, m_strImgRes.nHeight);
}

void CParser::ParseOP(char* chbuf)
{
	char outputimg[MAX_PATH] = {0};
	sscanf_s(chbuf,"%s %s",m_chLineType,10, outputimg,MAX_PATH);
	m_strOutputImg = outputimg;

	g_pRayTracer->SetOutputFileName(m_strOutputImg);
}

void CParser::ParseBK(char* chbuf)
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	sscanf_s(chbuf,"%s %f %f %f",m_chLineType,10,&r,&g,&b);
	m_vecBkg.push_back(r);
	m_vecBkg.push_back(g);
	m_vecBkg.push_back(b);

	M3DVector3f vBackground = { r, g, b };

	g_pRayTracer->SetBackground(vBackground);
}

void CParser::ParseRD(char* chbuf)
{
	sscanf_s(chbuf,"%s %d",m_chLineType,10,&m_nRdepth);

	g_pRayTracer->SetRecurDepth(m_nRdepth);
}

