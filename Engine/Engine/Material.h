#pragma once
#include "SimpleShader.h"
class Material
{
public:
	Material();
	Material(SimpleVertexShader* vShader, SimplePixelShader* pShader); 
	~Material();
	
	SimpleVertexShader* vertexShader; 
	SimplePixelShader* pixelShader; 
};

