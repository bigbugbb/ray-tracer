#pragma once

#include <GLTools.h>
#include "IPrimitiveFactory.h"

#include <vector>
using std::vector;

// base class for all other primitives
class CPrimitive
{
public:
	CPrimitive(void);
	virtual ~CPrimitive(void);

	// set different materials
	void SetAmbientMaterial(M3DVector3f vColor);
	void SetDiffuseMaterial(M3DVector3f vColor);
	void SetSpecularMaterial(M3DVector3f vColor, GLint nShininess);
	void SetTransmissiveMaterial(M3DVector3f vColor, GLint nIOR);

	// get different materials
	void GetAmbientMaterial(M3DVector3f vColor);
	void GetDiffuseMaterial(M3DVector3f vColor);
	void GetSpecularMaterial(M3DVector3f vColor, GLint& nShininess);
	void GetTransmissiveMaterial(M3DVector3f vColor, GLint& nIOR);

	// hit note operation
	const vector<GLfloat>& GetHitNote();
	void ClearHitNote();

	// get the primitive type
	PrimitiveType GetType();
	
	// get the normal vector at the specific point
	virtual void GetNormal(M3DVector3f vNormal, M3DVector3f vPoint) = 0;
	// do intersect test of the primitive, used for object selection and light tracing
	virtual BOOL Intersect(const M3DVector3f vRayOrgin, const M3DVector3f vRayDir, GLfloat fMinT, GLfloat fMaxT) = 0;

protected:
	PrimitiveType	m_eType;

	M3DVector3f		m_vAmbient;
	M3DVector3f		m_vDiffuse;
	M3DVector3f		m_vSpecular;
	M3DVector3f		m_vTransmissive;
	GLint			m_nShininess;
	GLint			m_nIOR;
	vector<GLfloat> m_vecHitNote;
};

