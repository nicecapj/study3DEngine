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
		sizeof(PIXELFORMATDESCRIPTOR),    // 이 픽셀포맷 설명자의 크기
		1,                                // 버전
		PFD_DRAW_TO_WINDOW |              // 포맷이 반드시 윈도우를 지원해야 함
		PFD_SUPPORT_OPENGL |              // 포맷이 반드시 OpenGL을 지원해야 함
		PFD_DOUBLEBUFFER,                 // 반드시 더블 버퍼링을 지원해야함
		PFD_TYPE_RGBA,                    // RGBA 포맷을 요청
		colorBit,                             // 색상깊이를 선택
		0, 0, 0, 0, 0, 0,                        // 색상비트 무시
		0,                                // 알파버퍼 없음
		0,                                // 쉬프트 비트 무시
		0,                                // Accumulation 버퍼 없음
		0, 0, 0, 0,                            // Accumulation 비트 무시
		16,                                // 16비트 Z-버퍼 (깊이 버퍼)
		0,                                // 스텐실 버퍼 없음
		0,                                // Auxiliary 버퍼 없음
		PFD_MAIN_PLANE,                            // 메인 드로잉 레이어
		0,                                // 예약됨
		0, 0, 0                                // 레이어 마스크 무시
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

