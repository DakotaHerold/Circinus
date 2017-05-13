#pragma once
#include "Config.h"

#ifdef EDITOR_BUILD

#include "PrimitiveRenderer.h"

class GizmoRenderer
{
private:
	static GizmoRenderer* _instance;

public:
	static GizmoRenderer* instance() { return _instance; }
	static PrimitiveRenderer* opaque() { return _instance->pr1; }
	static PrimitiveRenderer* xray() { return _instance->pr2; }

public:
	void Render(const DirectX::XMFLOAT4X4& matView, const DirectX::XMFLOAT4X4& matProj);

private:

	friend class RenderingSystem;

	void Init(ID3D11Device* device, ID3D11DeviceContext* context);
	void CleanUp();

private:
	PrimitiveRenderer*	pr1;
	PrimitiveRenderer*	pr2;
};

#endif