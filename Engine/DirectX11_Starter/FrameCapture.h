#pragma once

struct IDXGraphicsAnalysis;

class FrameCapture
{
public:
	static FrameCapture* instance();

	static void CleanUp();

public:

	// framesToCapture == 0, nothing happen
	void BeginCapture();

	void EndCapture();

	void SetFramesToCapture(unsigned int framesToCapture) { this->framesToCapture = framesToCapture; }

private:

	void Release();

	FrameCapture();

	IDXGraphicsAnalysis* ptr;

	unsigned int framesToCapture;
};