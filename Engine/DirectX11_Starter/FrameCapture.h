#pragma once

struct IDXGraphicsAnalysis;

class FrameCapture
{
public:
	static FrameCapture* instance();

	static void CleanUp();

	void BeginCapture();

	void EndCapture();

	void Release();

	void SetFramesToCapture(unsigned int framesToCapture) { this->framesToCapture = framesToCapture; }

private:
	FrameCapture();

	IDXGraphicsAnalysis* ptr;

	unsigned int framesToCapture;
};