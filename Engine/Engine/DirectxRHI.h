
#include "RHI.h"
#include <d3d9.h>

class DirectXRHI : public RHI
{
public:
	DirectXRHI();
	~DirectXRHI();

	virtual bool Initialize(HWND hwnd) override;

	virtual void Framemove(float delta) override;

	virtual bool Render() override;

	virtual bool Restore() override;


	bool InitializeGeometry();
	void RenderGeometry();
	void ReleaseGeometry();

private:
	LPDIRECT3D9              pD3D = NULL;   // Direct 3D에 접근
	LPDIRECT3DDEVICE9        pD3DDev = NULL;   // 비디오카드에 접근

	LPDIRECT3DVERTEXBUFFER9  pVB = NULL;
		
	struct CUSTOM_VERTEX
	{
		FLOAT X, Y, Z, RHW;
		DWORD Color;
	};
	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
};
