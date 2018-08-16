#include "stdafx.h"
//#include "S3DMesh_Directx.h"
//
//S3DMesh::S3DMesh(LPCWSTR pFilename, LPDIRECT3DDEVICE9 pD3DDevice, HWND hWnd)
//{
//	pD3DDevice_ = pD3DDevice;
//
//	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;
//	if (FAILED(D3DXLoadMeshFromX(pFilename, D3DXMESH_SYSTEMMEM, pD3DDevice, NULL, &pD3DXMtrlBuffer, NULL, &materialCount_, &mesh_)))
//	{
//		MessageBox(hWnd, L"Fail to Loading Object", L"", MB_OK);
//		return;
//	}
//
//	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
//	if (d3dxMaterials == nullptr)
//		return;
//
//	Materials = new D3DMATERIAL9[materialCount_];
//	textures_ = new LPDIRECT3DTEXTURE9[materialCount_];
//
//	for (DWORD i = 0; i < materialCount_; ++i)
//	{
//		Materials[i] = d3dxMaterials[i].MatD3D;
//		Materials[i].Ambient = Materials[i].Diffuse;	// Set the ambient color for the material (D3DX does not do this)
//
//		textures_[i] = nullptr;
//
//		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
//		{
//			const CHAR* strPreFix = "sampleResource/";
//			CHAR strTexture[MAX_PATH];
//			strcpy_s(strTexture, MAX_PATH, strPreFix);
//			strcat_s(strTexture, d3dxMaterials[i].pTextureFilename);
//
//			if (FAILED(D3DXCreateTextureFromFileA(pD3DDevice, strTexture, &textures_[i])))
//			{
//				MessageBox(hWnd, L"Fail to Loading Texture", L"", MB_OK);
//			}
//		}
//	}
//
//	pD3DXMtrlBuffer->Release();
//}
//
//S3DMesh::~S3DMesh()
//{
//	if(mesh_)
//	{
//		mesh_->Release();
//	}
//	delete[] Materials;
//
//	for (DWORD i = 0; i < materialCount_; ++i)
//	{
//		textures_[i]->Release();
//	}
//	delete[] textures_;
//}
//
//bool S3DMesh::Render()
//{
//	for (DWORD i = 0; i < materialCount_; ++i)
//	{
//
//		pD3DDevice_->SetMaterial(&Materials[i]);
//		pD3DDevice_->SetTexture(0, textures_[i]);
//
//		if (mesh_ != nullptr)
//		{
//			mesh_->DrawSubset(i);
//		}
//	}
//
//	return true;
//}
