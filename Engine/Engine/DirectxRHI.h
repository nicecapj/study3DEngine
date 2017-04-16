
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

private:
	LPDIRECT3D9              pD3D = NULL;   // Direct 3D�� ����
	LPDIRECT3DDEVICE9        pD3DDev = NULL;   // ����ī�忡 ����
};
