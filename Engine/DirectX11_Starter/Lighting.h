#pragma once
#include "Component.h"
#include "Lights.h"
class Lighting :
	public Component
{
private:
	Light *light;
	bool isDirty;
public:
	Lighting(XMFLOAT4 position, XMFLOAT4 color, int lightType, int enabled, int specularAmount);
	Lighting(XMFLOAT4 direction, XMFLOAT4 color, int lightType, int enabled);
	Lighting(XMFLOAT4 position, XMFLOAT4 direction, XMFLOAT4 color, float spotAngle, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, int lightType, int enabled, int specularAmount);
	~Lighting();

	Light& GetLight();
	bool WasModified();			// Returns true if the light was modified this frame
	void Cleanse();
};

