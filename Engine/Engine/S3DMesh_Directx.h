#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class S3DMesh
{
public:
	S3DMesh(LPCWSTR pFilename, LPDIRECT3DDEVICE9 pD3DDevice, HWND hWnd);
	~S3DMesh();

	bool Render();

	bool IsValidMesh()
	{
		return mesh_ != nullptr;
	}

	DWORD GetMaterialCount() {
		return materialCount_;
	}

	D3DMATERIAL9* GetMaterial(int materialIndex) {
		return &Materials[materialIndex];
	}

	IDirect3DTexture9* GetTexture(int materialIndex) {
		return textures_[materialIndex];
	};
private:
	LPD3DXMESH          mesh_ = nullptr;
	D3DMATERIAL9*       Materials = nullptr;
	LPDIRECT3DTEXTURE9* textures_ = nullptr;

	LPDIRECT3DDEVICE9   pD3DDevice_ = nullptr;

	DWORD               materialCount_ = 0;
};
