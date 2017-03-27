#pragma once
#include "SimpleShader.h"
#include "Texture.h"

class Material
{
public:
	Material();
	Material(SimpleVertexShader* vShader, SimplePixelShader* pShader); 
	~Material();
	
	SimpleVertexShader* vertexShader; 
	SimplePixelShader* pixelShader;
	Texture* texDiffuse;
	ID3D11SamplerState* sampBasic;
};

