#include "stdafx.h"
#include <stdexcept>
#include "OpenGLRHI.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

#pragma comment (lib, "opengl32.lib")  /* link with Microsoft OpenGL lib */
#pragma comment (lib, "glu32.lib")     /* link with OpenGL Utility lib */
#pragma comment (lib, "glut32.lib")    /* link with Win32 GLUT lib */


OpenGLRHI::OpenGLRHI()
{	
}

OpenGLRHI::~OpenGLRHI()
{
	//wglMakeCurrent(hDC_, nullptr);
	//wglDeleteContext(hRC_);
}

bool OpenGLRHI::Initialize(HWND hwnd)
{
	//hDC_ = GetDC(hwnd);
	//if (hDC_)
	//{
	//	hRC_ = wglCreateContext(hDC_);
	//	if (hRC_)
	//	{
	//		wglMakeCurrent(hDC_, hRC_);

	//		SetPixelFormat();
	//	}
	//}

	glutCreateWindow("study opengl engine");
	glutDisplayFunc(OpenGLRHI::TestGlutRender);
	glutMainLoop();

	return true;
}

void OpenGLRHI::TestGlutRender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glVertex2f(0.0, 0.5);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);

	glEnd();
	glFlush();
}

void OpenGLRHI::Framemove(float delta)
{

}

bool OpenGLRHI::Render()
{
	if (hRC_)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
	
		glVertex2f(0.0, 0.5);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0.5, -0.5);

		glEnd();
		glFlush();

		return true;
	}
	return false;
}

void OpenGLRHI::SetPixelFormat()
{

}

bool OpenGLRHI::Restore()
{
	glViewport(0, 0, windowWidth_, windowHeight_);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)windowWidth_ / windowHeight_, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	return true;
}
