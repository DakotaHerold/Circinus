#include "GizmoRenderer.h"

#ifdef HAS_EDITOR

#include <algorithm>
#include <PrimitiveBatch.h>
#include <d3dx11effect.h>

using namespace DirectX;

GizmoRenderer* GizmoRenderer::_instance = nullptr;

void GizmoRenderer::Render(const DirectX::XMFLOAT4X4& matView, const DirectX::XMFLOAT4X4& matProj)
{
	pr1->Render(matView, matProj, true);
	pr2->Render(matView, matProj, false);
}

void GizmoRenderer::Init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	assert(nullptr == _instance);

	pr1 = new PrimitiveRenderer();
	pr2 = new PrimitiveRenderer();

	pr1->Init(device, context);
	pr2->Init(device, context);

	_instance = this;
}

void GizmoRenderer::CleanUp()
{
	pr1->CleanUp();
	pr2->CleanUp();
	delete pr1;
	delete pr2;
	pr1 = nullptr;
	pr2 = nullptr;
}

#endif
