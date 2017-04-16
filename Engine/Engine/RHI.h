#pragma once
class RHI
{
public:
	RHI();
	virtual ~RHI();

	virtual bool Initialize(HWND hwnd) = 0;
	virtual void Framemove(float delta) = 0;
	virtual bool Render() = 0;
	virtual bool Restore() = 0;
};

