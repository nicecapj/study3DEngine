#pragma once
class RHI
{
public:
	RHI();
	virtual ~RHI();

	virtual bool Initialize(HWND hwnd, int width, int height) = 0;
	virtual void Framemove(float delta) = 0;
	virtual bool Render() = 0;
	
	virtual bool Invalidate() = 0;	//lost device
	virtual bool Restore() = 0;		//recovered from reset device	
};

