
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
	HWND hWnd = nullptr;
	LPDIRECT3D9              pD3D = nullptr;		  // Direct 3D�� ����
	LPDIRECT3DDEVICE9        pD3DDevice = nullptr;   // ����ī�忡 ����

	LPDIRECT3DVERTEXBUFFER9  pVB = nullptr;
	LPDIRECT3DTEXTURE9       pTexture = nullptr;
		
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
		FLOAT u, v;
	};
	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
};
