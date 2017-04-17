#pragma once
#include "RHI.h"
#include <windows.h>

class OpenGLRHI :public RHI
{
public:
	OpenGLRHI();	
	~OpenGLRHI();

	virtual bool Initialize(HWND hwnd) override;
	virtual void Framemove(float delta) override;
	virtual bool Render() override;
	virtual bool Restore() override;

	
	static void TestGlutRender();
	void SetPixelFormat();
	

private:

	HDC hDC_;
	HGLRC hRC_;
	int windowWidth_ = 0;
	int windowHeight_ = 0;
};

