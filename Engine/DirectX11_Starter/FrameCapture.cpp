#include "FrameCapture.h"

#if defined(_DEBUG)
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <DXProgrammableCapture.h>
#pragma comment(lib, "dxgi.lib")
#endif

FrameCapture* FrameCapture::instance()
{
	static FrameCapture* _instance = new FrameCapture();
	return _instance;
}

void FrameCapture::CleanUp()
{
	FrameCapture* ptr = instance();
	ptr->Release();
	delete ptr;
}

void FrameCapture::BeginCapture()
{
#if defined(_DEBUG)
	if (nullptr != ptr && framesToCapture > 0)
		ptr->BeginCapture();
#endif
}

void FrameCapture::EndCapture()
{
#if defined(_DEBUG)
	if (nullptr != ptr && framesToCapture > 0)
	{
		ptr->EndCapture();
		framesToCapture--;
	}
#endif
}

void FrameCapture::Release()
{
#if defined(_DEBUG)
	if (nullptr != ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
#endif
}

FrameCapture::FrameCapture()
	:
	ptr(nullptr),
	framesToCapture(0)
{
#if defined(_DEBUG)
	HRESULT hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&ptr));
	if (hr != S_OK)
	{
		ptr = nullptr;
	}
#endif
}
