
#include "RHI.h"
#include <d3d9.h>
#include <d3dx9.h>

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
		
	void SetupLights();
	void SetupMatrices();// Setup the world, view, and projection matrices

private:
	LPDIRECT3D9              pD3D = NULL;		  // Direct 3D에 접근
	LPDIRECT3DDEVICE9        pD3DDevice = NULL;   // 비디오카드에 접근

	LPDIRECT3DVERTEXBUFFER9  pVB = NULL;
		
	//struct CUSTOM_VERTEX
	//{
	//	FLOAT X, Y, Z, RHW;
	//	DWORD Color;
	//};

	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 Position; // The 3D position for the vertex
		D3DXVECTOR3 Normal;   // The surface normal for the vertex
		DWORD Color;
	};
	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)
};
