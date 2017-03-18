#include "Material.h"



Material::Material()
{
	vertexShader = nullptr; 
	pixelShader = nullptr; 
}

Material::Material(SimpleVertexShader * vShader, SimplePixelShader* pShader)
{
	vertexShader = vShader; 
	pixelShader = pShader; 
}


Material::~Material()
{
}
