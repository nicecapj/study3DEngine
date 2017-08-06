#pragma once

class RHI;
class S3DEngine
{
public:
	enum EngineType
	{
		OPENGL,
		DIRECTX,
	};
		
	bool IsInitialized();
	bool Initialize(HWND hwnd);
	void Process();
	bool Restore();

	S3DEngine(EngineType engineType);
	virtual ~S3DEngine();			

protected:	
	RHI* Create(EngineType engineType);
	RHI* rhi_ = nullptr;	

private:
	bool isInitialized_ = false;
	
};