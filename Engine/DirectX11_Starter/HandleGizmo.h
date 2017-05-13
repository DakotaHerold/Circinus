#pragma once
#include "Config.h"

#ifdef HAS_EDITOR

#include <DirectXMath.h>

class Entity;

class HandleGizmo
{
public:
	enum Mode
	{
		Translate,
		Rotate,
		Scale
	};
public:
	HandleGizmo()
		:
		selX(false),
		selY(false),
		selZ(false),
		mode(Translate)
	{}

	void SetMode(Mode mode) { this->mode = mode; }
	Mode GetMode() const { return mode; }

	void _vectorcall Update(
		Entity* entity, 
		float deltaTime, 
		float totalTime,
		DirectX::FXMVECTOR rayStart, 
		DirectX::FXMVECTOR rayDir,
		DirectX::FXMVECTOR mouseWorldDelta,
		DirectX::FXMMATRIX matVP);

	bool IsMoving() const { return selX || selY || selZ; }

private:
	void UpdateTranslator();
	void UpdateTranslator(Entity* e, float deltaX, float deltaY, float deltaZ);

	void _vectorcall DrawTranslator(
		DirectX::XMVECTOR worldPos, 
		DirectX::XMVECTOR worldXDir, 
		DirectX::XMVECTOR worldYDir, 
		DirectX::XMVECTOR worldZDir);

	void UpdateRotator();
	void UpdateRotator(Entity* e, float deltaX, float deltaY, float deltaZ);

	void _vectorcall DrawRotator(
		DirectX::XMVECTOR worldPos,
		DirectX::XMVECTOR worldXDir,
		DirectX::XMVECTOR worldYDir,
		DirectX::XMVECTOR worldZDir);

	void UpdateScalor();
	void UpdateScalor(Entity* e, float deltaX, float deltaY, float deltaZ);

	void _vectorcall DrawScalor(
		DirectX::XMVECTOR worldPos,
		DirectX::XMVECTOR worldXDir,
		DirectX::XMVECTOR worldYDir,
		DirectX::XMVECTOR worldZDir);

private:
	DirectX::XMFLOAT3		hitXY;
	DirectX::XMFLOAT3		hitXZ;
	DirectX::XMFLOAT3		hitYZ;
	bool					selX;
	bool					selY;
	bool					selZ;
	Mode					mode;
};

#endif
