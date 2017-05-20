#pragma once
#include "Component.h"
#include "Lights.h"
class Lighting :
	public Component
{
private:
	Light *light;
	bool isDirty;		// boolean value denoting if object Lighting object was modified after the previous update and before the next update
public:

	// Constructor for point light
	Lighting(XMFLOAT4 position, XMFLOAT4 color, int lightType, int enabled, int specularAmount);

	// Constructor for Directional light
	Lighting(XMFLOAT4 direction, XMFLOAT4 color, int lightType, int enabled);

	// Common constructor
	Lighting(XMFLOAT4 position, XMFLOAT4 direction, XMFLOAT4 color, float spotAngle, float constantAttenuation, float linearAttenuation, float quadraticAttenuation, int lightType, int enabled, int specularAmount);

	//Empty constructor
	Lighting();
	void Init(XMFLOAT4 position, XMFLOAT4 color, int lightType, int enabled, int specularAmount) {
		light->Position = position;
		light->Color = color;
		light->LightType = lightType;
		light->Enabled = enabled;
		light->SpecularAmount = specularAmount;
	}
	~Lighting();

	Light& GetLight();
	bool WasModified();			// Returns true if the light was modified this frame
	void SetModified() { isDirty = true; }
	void Cleanse();				// Sets isDirty to false
	void SetPosition(XMFLOAT3& pos);	
	void SetRotationEuler(XMFLOAT3& rot);

	// Serialization code for light component (Serialization done only for point light at the moment)
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

