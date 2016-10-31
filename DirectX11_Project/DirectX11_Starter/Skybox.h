#pragma once
#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Entity.h"
class Skybox
{
public:
	Skybox();
	Skybox(Entity* box); 
	~Skybox();
	Entity* skyBox;
	void Draw(ID3D11DeviceContext* deviceContext);
};

