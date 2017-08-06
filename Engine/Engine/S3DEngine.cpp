#include "stdafx.h"
#include "S3DEngine.h"
#include "OpenGLRHI.h"
#include "DirectxRHI.h"

S3DEngine::S3DEngine(EngineType engineType)
{
	rhi_ = Create(engineType);
}

S3DEngine::~S3DEngine()
{
	delete rhi_;
	rhi_ = nullptr;
}


RHI* S3DEngine::Create(EngineType engineType)
{
	switch (engineType)
	{
	case OPENGL: 
	{
		return new OpenGLRHI; 
	}
	case DIRECTX: 
	{
		return new DirectXRHI;
	}

	default:
		break;
	}

	return nullptr;
}

void S3DEngine::Process()
{
	if (rhi_)
	{
		rhi_->Framemove(0);
		rhi_->Render();		
	}
}

bool S3DEngine::Restore()
{
	if (!rhi_)
		return false;

	return rhi_->Restore();
}

bool S3DEngine::Initialize(HWND hwnd, int width, int height)
{
	if (!rhi_)
		return false;

	isInitialized_ = rhi_->Initialize(hwnd, width, height);
	return isInitialized_;
}

bool S3DEngine::IsInitialized()
{
	return isInitialized_;
}