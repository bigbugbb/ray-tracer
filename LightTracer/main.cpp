#include "stdafx.h"
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

#include <math.h>
#include <stdio.h>

#include "RayTracer.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif


//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	glEnable(GL_DEPTH_TEST);
    
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


///////////////////////////////////////////////////
// Screen changes size or is initialized
void ChangeSize(int nWidth, int nHeight)
{
	glViewport(0, 0, nWidth, nHeight);
}

        
// Called to draw scene
void RenderScene(void)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Ray tracing
	g_pRayTracer->Render();
	
	// Read data from back buffer and save it into a bmp file
	g_pRayTracer->Save();

    // Do the buffer Swap
    glutSwapBuffers();
        
    // Tell GLUT to do it again
    //glutPostRedisplay();
}


// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
{
	//float linear = 0.1f;
	//float angular = float(m3dDegToRad(5.0f));
	//
	//if (key == GLUT_KEY_UP)
	//	cameraFrame.MoveForward(linear);
	//
	//if (key == GLUT_KEY_DOWN)
	//	cameraFrame.MoveForward(-linear);
	//
	//if (key == GLUT_KEY_LEFT)
	//	cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
	//
	//if (key == GLUT_KEY_RIGHT)
	//	cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
}

int main(int argc, char* argv[])
{
	GLint nWidth = 512, nHeight = 512;
	string strFilePath = "c:/2.rts";

	gltSetWorkingDirectory(argv[0]);

	if (argc == 2) {
		strFilePath = argv[1];
	} else {
		fprintf(stderr, "No input file is found, choose the default one to parse.\n");
	}

	g_pRayTracer->Load(strFilePath); //"C:/Users/bigbug/Desktop/parse.txt");
	g_pRayTracer->Prepare();
	g_pRayTracer->GetResolution(nWidth, nHeight);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(nWidth, nHeight);
  
    glutCreateWindow("Ray Tracer");
 
    glutSpecialFunc(SpecialKeys);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }		

    SetupRC();
    glutMainLoop();    
    return 0;
}
