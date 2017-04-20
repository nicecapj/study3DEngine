#include "stdafx.h"
#include "DirectxRHI.h"
#include <mmsystem.h>
#include <stdexcept>


DirectXRHI::DirectXRHI()
{	
	objects_;
}


DirectXRHI::~DirectXRHI()
{
	ReleaseGeometry();

	pD3DDevice->Release();
	pD3DDevice = nullptr;
	pD3D->Release();
	pD3D = nullptr;
}

bool DirectXRHI::Initialize(HWND hWnd)
{
	this->hWnd = hWnd;

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
		&d3dpp, &pD3DDevice))) 
		return false;

	// Turn off culling, so we see the front and back of the triangle
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting, since we are providing our own vertex colors
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on the zbuffer
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

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
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	pD3DDevice->BeginScene(); // 렌더 시작
	
	SetupLights();
	SetupMatrices();
	RenderGeometry();

	pD3DDevice->EndScene(); // 렌더 종료

	// 출력한 내용을 실제 윈도우에 나타나게 한다
	if (FAILED(pD3DDevice->Present(NULL, NULL, NULL, NULL)))
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
#ifdef TEST_VERTEXBUFFER
	if (FAILED(pD3DDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &pVB, NULL)))
	{
		return false;
	}

	CUSTOMVERTEX* pVertices;
	if (FAILED(pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return false;

	bool textureSet = false;
	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);
		pVertices[2 * i + 0].Position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		pVertices[2 * i + 0].Normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		pVertices[2 * i + 0].Color = D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff);

		pVertices[2 * i + 0].u = textureSet ? 1.0f : 0.0f;
		pVertices[2 * i + 0].v = textureSet ? 0.0f : 0.0f;

		pVertices[2 * i + 1].Position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
		pVertices[2 * i + 1].Normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		pVertices[2 * i + 1].Color = D3DCOLOR_RGBA(0x00, 0xff, 0x00, 0xff);
		pVertices[2 * i + 0].u = textureSet ? 1.0f : 0.0f;
		pVertices[2 * i + 0].v = textureSet ? 1.0f : 1.0f;
		textureSet = !textureSet;
	}

	pVB->Unlock();

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, L"sampleResource/sample.bmp", &pTexture)))
	{
		MessageBox(hWnd, L"Fail to Loading Texture", L"", MB_OK);
		return false;
	}
#endif

	S3DMesh* pMesh = new S3DMesh();
	objects_.push_back(pMesh);

	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;		
	if (FAILED(D3DXLoadMeshFromX(L"sampleResource/tiger.x", D3DXMESH_SYSTEMMEM, pD3DDevice, NULL, &pD3DXMtrlBuffer, NULL, &(pMesh->MeterialCount), &(pMesh->Mesh))))
	{
		MessageBox(hWnd, L"Fail to Loading Object", L"", MB_OK);
		return false;
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMesh->Materials = new D3DMATERIAL9[pMesh->MeterialCount];
	pMesh->Textures = new LPDIRECT3DTEXTURE9[pMesh->MeterialCount];

	for (DWORD i = 0; i < pMesh->MeterialCount; ++i)
	{
		pMesh->Materials[i] = d3dxMaterials[i].MatD3D;
		pMesh->Materials[i].Ambient = pMesh->Materials[i].Diffuse;	// Set the ambient color for the material (D3DX does not do this)

		pMesh->Textures[i] = nullptr;

		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			const CHAR* strPreFix = "sampleResource/";
			CHAR strTexture[MAX_PATH];
			strcpy_s(strTexture, MAX_PATH, strPreFix);
			strcat_s(strTexture, d3dxMaterials[i].pTextureFilename);

			if (FAILED(D3DXCreateTextureFromFileA(pD3DDevice, strTexture, &(pMesh->Textures[i]))))
			{
				MessageBox(hWnd, L"Fail to Loading Texture", L"", MB_OK);
				return false;
			}
		}

	}

	pD3DXMtrlBuffer->Release();

	return true;
}

void DirectXRHI::RenderGeometry()
{
#ifdef TEST_VERTEXBUFFER
	//texture * diffuse
	pD3DDevice->SetTexture(0, pTexture);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTS_TEXTURE1);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);


	pD3DDevice->SetStreamSource(0, pVB, 0, sizeof(CUSTOMVERTEX));
	pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	pD3DDevice->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST, 0, 1);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);
#endif

	for (auto obj : objects_)
	{
		for (DWORD i = 0; i < obj->MeterialCount; ++i)
		{
			pD3DDevice->SetMaterial(&(obj->Materials[i]));
			pD3DDevice->SetTexture(0, obj->Textures[i]);
			
			if(obj->Mesh)
			{
				obj->Mesh->DrawSubset(i);
			}			
		}
	}
}

void DirectXRHI::ReleaseGeometry()
{
	if (pVB)
	{
		pVB->Release();
		pVB = nullptr;
	}

	if (pTexture)
	{
		pTexture->Release();
		pTexture = nullptr;
	}

	for (auto it : objects_)
	{
		it->Mesh->Release();
		delete[] it->Materials;

		for (int i = 0; i < (int)it->MeterialCount ; ++i)
		{									
			it->Textures[i]->Release();
		}
		delete[] it->Textures;
		delete it;
	}
	objects_.clear();
}

void DirectXRHI::SetupMatrices()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixRotationY(&matWorld, fAngle);
	pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void DirectXRHI::SetupLights()
{
	// Set up a material. The material here just has the diffuse and ambient
	// colors set to yellow. Note that only one material can be used at a time.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pD3DDevice->SetMaterial(&mtrl);

	// Set up a white, directional light, with an oscillating direction.
	// Note that many lights may be active at a time (but each one slows down
	// the rendering of our scene). However, here we are just using one. Also,
	// we need to set the D3DRS_LIGHTING renderstate to enable lighting
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
		1.0f,
		sinf(timeGetTime() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	pD3DDevice->SetLight(0, &light);
	pD3DDevice->LightEnable(0, TRUE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Finally, turn on some ambient light.
	pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}
