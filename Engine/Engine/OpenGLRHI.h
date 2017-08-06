#pragma once
#include "RHI.h"
#include <windows.h>

class OpenGLRHI :public RHI
{
public:
	OpenGLRHI();	
	~OpenGLRHI();

	virtual bool Initialize(HWND hwnd, int width, int height) override;
	virtual void Framemove(float delta) override;
	virtual bool Render() override;
	virtual bool Restore() override;
	virtual bool Invalidate() override;	
		
	void CheckPixelFormat(HDC dc, BYTE colorBit);

	

	void TempErrorMessage(HWND hwnd);

private:
	void Resize(int width, int height);
	void DrawTestQuad();
	void DrawTestTriangle();

	HWND hWnd_;
	HDC hDC_;
	HGLRC hRC_;
	int windowWidth_ = 0;
	int windowHeight_ = 0;
};

