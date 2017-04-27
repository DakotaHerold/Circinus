#include "Lighting.h"



Lighting::Lighting(XMFLOAT4 position, XMFLOAT4 color, int lightType, int enabled, int specularAmount) :
	Lighting(position, XMFLOAT4(),color, 0, 1, 0, 0, lightType, enabled, specularAmount)
{
}

Lighting::Lighting(XMFLOAT4 direction, XMFLOAT4 color, int lightType, int enabled) :
	Lighting(XMFLOAT4(), direction, color, 0, 1, 0, 0, lightType, enabled, 0)
{
}

Lighting::Lighting(XMFLOAT4 position, XMFLOAT4 direction, XMFLOAT4 color, float spotAngle, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, int lightType, int enabled, int specularAmount)
{
	light = new Light();
	light->Position = position;
	light->Direction = direction;
	light->Color = color;
	light->SpotAngle = spotAngle;
	light->ConstantAttenuation = constantAttenuation;
	light->LinearAttenuation = linearAttenuation;
	light->QuadraticAttenuation = quadraticAttenuation;
	light->LightType = lightType;
	light->Enabled = enabled;
	light->SpecularAmount = specularAmount;
	isDirty = true;
}


Lighting::~Lighting()
{
	delete light;
}

Light & Lighting::GetLight()
{
	return *light;
}

bool Lighting::WasModified()
{
	return isDirty;
}

void Lighting::Cleanse()
{
	isDirty = false;
}
