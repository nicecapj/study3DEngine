#include "stdafx.h"
#include <stdexcept>
#include "OpenGLRHI.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>
#include <windows.h>

#pragma comment (lib, "opengl32.lib")  /* link with Microsoft OpenGL lib */
#pragma comment (lib, "glu32.lib")     /* link with OpenGL Utility lib */
#pragma comment (lib, "glut32.lib")    /* link with Win32 GLUT lib */


OpenGLRHI::OpenGLRHI()
{	
}

OpenGLRHI::~OpenGLRHI()
{
	wglMakeCurrent(hDC_, nullptr);
	wglDeleteContext(hRC_);
}

bool OpenGLRHI::Initialize(HWND hwnd)
{	
	hWnd_ = hwnd;
	hDC_ = GetDC(hWnd_);
	if (hDC_)
	{		
		CheckPixelFormat(hDC_, 32);

		hRC_ = wglCreateContext(hDC_);
		if (hRC_)
		{
			wglMakeCurrent(hDC_, hRC_);

			glShadeModel(GL_SMOOTH);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

			glClearDepth(1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);			

			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		}
		else
		{			
			TempErrorMessage(hWnd_);
		}
	}

	//glutCreateWindow("study opengl engine");
	//glutDisplayFunc(OpenGLRHI::TestGlutRender);
	//glutMainLoop();

	return true;
}

void OpenGLRHI::TestGlutRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		
		SwapBuffers(hDC_);

		return true;
	}
	return false;
}

void OpenGLRHI::CheckPixelFormat(HDC dc, int colorBit)
{
	static    PIXELFORMATDESCRIPTOR ppfd =                   
	{
		sizeof(PIXELFORMATDESCRIPTOR),    // �� �ȼ����� �������� ũ��
		1,                                // ����
		PFD_DRAW_TO_WINDOW |              // ������ �ݵ�� �����츦 �����ؾ� ��
		PFD_SUPPORT_OPENGL |              // ������ �ݵ�� OpenGL�� �����ؾ� ��
		PFD_DOUBLEBUFFER,                 // �ݵ�� ���� ���۸��� �����ؾ���
		PFD_TYPE_RGBA,                    // RGBA ������ ��û
		colorBit,                             // ������̸� ����
		0, 0, 0, 0, 0, 0,                        // �����Ʈ ����
		0,                                // ���Ĺ��� ����
		0,                                // ����Ʈ ��Ʈ ����
		0,                                // Accumulation ���� ����
		0, 0, 0, 0,                            // Accumulation ��Ʈ ����
		16,                                // 16��Ʈ Z-���� (���� ����)
		0,                                // ���ٽ� ���� ����
		0,                                // Auxiliary ���� ����
		PFD_MAIN_PLANE,                            // ���� ����� ���̾�
		0,                                // �����
		0, 0, 0                                // ���̾� ����ũ ����
	};
	
	GLuint pixelFormat;
	if (!(pixelFormat = ChoosePixelFormat(dc, &ppfd)))
	{
		TempErrorMessage(hWnd_);
		return;
	}

	SetPixelFormat(dc, pixelFormat, &ppfd);
}

void OpenGLRHI::TempErrorMessage(HWND hwnd)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.			
	::MessageBox(hwnd, (LPCTSTR)lpMsgBuf, TEXT(""), 0);
	// Free the buffer.
	LocalFree(lpMsgBuf);
}

void OpenGLRHI::Resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, windowWidth_, windowHeight_);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)windowWidth_ / windowHeight_, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool OpenGLRHI::Invalidate()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool OpenGLRHI::Restore()
{
	Resize(windowWidth_, windowHeight_);
	return true;
}

