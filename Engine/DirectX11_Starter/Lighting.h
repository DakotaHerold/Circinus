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
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		writer.StartObject();
		writer.String("name");
		writer.String("Lighting");
		writer.String("PositionX");
		writer.Double(light->Position.x);
		writer.String("PositionY");
		writer.Double(light->Position.y);
		writer.String("PositionZ");
		writer.Double(light->Position.z);
		writer.String("PositionW");
		writer.Double(light->Position.w);
		writer.String("ColorX");
		writer.Double(light->Color.x);
		writer.String("ColorY");
		writer.Double(light->Color.y);
		writer.String("ColorZ");
		writer.Double(light->Color.z);
		writer.String("ColorW");
		writer.Double(light->Color.w);
		writer.String("lightType");
		writer.Int(light->LightType);
		writer.String("enabled");
		writer.Int(light->Enabled);
		writer.String("specularAmount");
		writer.Int(light->SpecularAmount);
		writer.EndObject();
	}
	void Load(rapidjson::Value v) {
		
	}
};

