#pragma once
#include <memory>
#include "SimpleShader.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace std;

class HUD
{
public:
	HUD();
	HUD(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, float x, float y);
	HUD(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, const wchar_t* inputString, float x, float y);
	~HUD();

	void Update();
	void Render();
	XMFLOAT2 getPosition();
	void setPosition(float x, float y);
	void move(float x, float y);
	void changeText(const wchar_t* newText);

private:
	//SpriteBatch* spriteBatch; 
	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<SpriteFont> spriteFont;
	ID3D11ShaderResourceView* texture;
	XMFLOAT2 position;
	const wchar_t* text;
	bool isSpriteFont;
};