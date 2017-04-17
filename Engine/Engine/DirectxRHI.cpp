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
	// Direct 3D ���
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) 
		return false;

	// ���÷��� ��� ����
	D3DDISPLAYMODE d3ddm;
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return false;

	// Direct 3D ����̽� ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// ���� 0����
	d3dpp.Windowed = TRUE;                      // ��������
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;   // ȭ����ȯ ���
	d3dpp.BackBufferFormat = d3ddm.Format;      // ���� ȭ�� ���� ���

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
	// ����� �˰�ĥ�Ѵ�
	pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	pD3DDev->BeginScene(); // ���� ����

	RenderGeometry();

	pD3DDev->EndScene(); // ���� ����

	// ����� ������ ���� �����쿡 ��Ÿ���� �Ѵ�
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
