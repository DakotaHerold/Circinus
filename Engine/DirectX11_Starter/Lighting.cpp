#include "Lighting.h"



Lighting::Lighting(XMFLOAT4 position, XMFLOAT4 direction, XMFLOAT4 color, int lightType, int enabled) :
	Lighting(position, direction, color, 0, 1, 0, 0, lightType, enabled)
{
}

Lighting::Lighting(XMFLOAT4 position, XMFLOAT4 direction, XMFLOAT4 color, float spotAngle, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, int lightType, int enabled)
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
}


Lighting::~Lighting()
{
	delete light;
}

Light & Lighting::GetLight()
{
	return *light;
}
