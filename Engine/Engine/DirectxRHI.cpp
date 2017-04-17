#include "stdafx.h"
#include "DirectxRHI.h"
#include <stdexcept>


DirectXRHI::DirectXRHI()
{	
}


DirectXRHI::~DirectXRHI()
{
	ReleaseGeometry();

	pD3DDev->Release();
	pD3DDev = nullptr;
	pD3D->Release();
	pD3D = nullptr;
}

bool DirectXRHI::Initialize(HWND hWnd)
{
	// Direct 3D 사용
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) 
		return false;

	// 디스플레이 모드 조사
	D3DDISPLAYMODE d3ddm;
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return false;

	// Direct 3D 디바이스 생성
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// 몽땅 0으로
	d3dpp.Windowed = TRUE;                      // 윈도우모드
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;   // 화면전환 방법
	d3dpp.BackBufferFormat = d3ddm.Format;      // 현재 화면 포맷 사용

	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &pD3DDev))) 
		return false;

	if (!InitializeGeometry())
		return false;

	return true;
}

void DirectXRHI::Framemove(float delta)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

bool DirectXRHI::Render()
{
	// 배경을 검게칠한다
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	pD3DDev->BeginScene(); // 렌더 시작

	RenderGeometry();

	pD3DDev->EndScene(); // 렌더 종료

	// 출력한 내용을 실제 윈도우에 나타나게 한다
	if (FAILED(pD3DDev->Present(NULL, NULL, NULL, NULL)))
		return false;

	return true;
}

bool DirectXRHI::Restore()
{
	//throw std::logic_error("The method or operation is not implemented.");
	return true;
}

bool DirectXRHI::InitializeGeometry()
{
	if (FAILED(pD3DDev->CreateVertexBuffer(3 * sizeof(CUSTOM_VERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &pVB, NULL)))
	{
		return false;
	}

	CUSTOM_VERTEX vertics[] =
	{
		{150.f, 50.f, 0.5f, 1.0f, D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff) },
		{250.f, 250.f, 0.5f, 1.0f, D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff) },
		{50.f, 250.f, 0.5f, 1.0f, D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff) },
	};

	void* pVertics = nullptr;

	if (FAILED(pVB->Lock(0, sizeof(vertics), &pVertics, 0)))
		return false;

	memcpy(pVertics, vertics, sizeof(vertics));
	memcpy(pVertics, &vertics[0], sizeof(vertics));

	pVB->Unlock();

	return true;
}

void DirectXRHI::RenderGeometry()
{
	pD3DDev->SetStreamSource(0, pVB, 0, sizeof(CUSTOM_VERTEX));
	pD3DDev->SetFVF(D3DFVF_CUSTOMVERTEX);
	pD3DDev->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 1);
}

void DirectXRHI::ReleaseGeometry()
{
	pVB->Release();
	pVB = nullptr;
}
