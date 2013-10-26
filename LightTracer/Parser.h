#ifndef _2DDRAW_HHH
#define _2DDRAW_HHH

#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

typedef vector<float> vecfloat;
typedef vector<int> vecint;

//Image Resolution struct
struct struImgRes
{
	int nWidth;
	int nHeight;
};

//some materials struct
struct struMaterial
{
	vecfloat vecAmbientMaterial;
	vecfloat vecDiffuseMaterial;
	vecfloat vecSpecularMaterial;
//  vecfloat vecTransmissiveMaterial;
};

//some lights struct
struct struLight
{
	vecfloat vecPointLight;
	vecfloat vecDirectionalLight;
	vecfloat vecSpotLight;
	vecfloat vecAmbientLight;
};

class CParser
{
public:
	//input the path of file that need to be parsed
	CParser(const char* chPath);
	~CParser();

private:
	//parse for the first time, get all vertex
	void FirstPass();
	//parse for the second time, get all vertex
	void SecondPass();
		
public:
	//parse
	void Parse();

public:
	void GetVertex(vector<vecfloat>& vv);
	void GetMaterial(struMaterial& mt);
	float GetSphere();
	void GetTriangle(vecfloat& vf);
	float GetPlane();
	void GetLight(struLight& lt);
	float GetCamera();
	void GetImgRes(struImgRes& ir);
	const char* GetOutputImg();
	void GetBack(vecfloat& vecBkg);
	int GetRDepth();

private:
	//Get Line Type
	void GetLineType(char* cstr);
	//Parse one line
	void ParseOneLine(char* chbuf); 

private:
	//parse functions for each line type
	void ParseVV(char* chbuf);
	void ParseAM(char* chbuf);
	void ParseDM(char* chbuf);
	void ParseSM(char* chbuf);
	void ParseTM(char* chbuf);
	void ParseSS(char* chbuf);
	void ParseTS(char* chbuf);
	void ParsePS(char* chbuf);
	void ParsePL(char* chbuf);
	void ParseDL(char* chbuf);
	void ParseSL(char* chbuf);
	void ParseAL(char* chbuf);
	void ParseCC(char* chbuf);
	void ParseIR(char* chbuf);
	void ParseOP(char* chbuf);
	void ParseBK(char* chbuf);
	void ParseRD(char* chbuf);

private:
	string m_strPath;

private:
	vector<vecfloat> m_vecVertex;
	struMaterial	 m_struMaterial;
	float			 m_fPlane; 
	struLight		 m_struLight;
	float			 m_fCamera;
	struImgRes		 m_strImgRes;
	string			 m_strOutputImg;
	vecfloat		 m_vecBkg;
	int				 m_nRdepth;

private:
	//line type
	char m_chLineType[10];
};

#endif
