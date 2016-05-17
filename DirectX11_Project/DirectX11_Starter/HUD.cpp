#include "HUD.h"



	HUD::HUD()
{
	spriteBatch = nullptr;
	texture = nullptr;
	XMFLOAT2 position = XMFLOAT2(0, 0);
}

//HUD::HUD(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* SRV, const wchar_t* fileName,  float x, float y)

HUD::HUD(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* fileName, float x, float y)
{
	//Initialize values 
	position = XMFLOAT2(x, y);

	// Create Sprite Batch Object 
	spriteBatch.reset(new SpriteBatch(deviceContext));

	// Import texture for rendering 
	CreateDDSTextureFromFile(device, fileName, nullptr, &texture);

	//No Font 
	spriteFont = nullptr;
	text = nullptr;

	isSpriteFont = false;
}

HUD::HUD(ID3D11Device * device, ID3D11DeviceContext * deviceContext, const wchar_t * fileName, const wchar_t * inputString, float x, float y)
{
	//Initialize values 
	position = XMFLOAT2(x, y);
	text = inputString;

	// Create Sprite Batch Object 
	spriteBatch.reset(new SpriteBatch(deviceContext));

	//No Texture
	texture = nullptr;

	//Font 
	spriteFont.reset(new SpriteFont(device, fileName));

	isSpriteFont = true;
}


HUD::~HUD()
{

	if (isSpriteFont)
	{
		texture = nullptr;
		delete texture;
	}
	else
	{
		//Memory::SafeRelease(texture) ?? 
		texture->Release();
	}

	text = nullptr;
	delete text;
}

void HUD::Update()
{

}

void HUD::Render()
{
	spriteBatch->Begin();

	//Draw Sprites, Fonts, etc. 
	if (isSpriteFont == false)
	{
		spriteBatch->Draw(texture, position);
	}
	else
	{
		spriteFont->DrawString(spriteBatch.get(), text, position);
	}

	spriteBatch->End();
}

XMFLOAT2 HUD::getPosition()
{
	return position;
}

void HUD::setPosition(float x, float y)
{
	position = XMFLOAT2(x, y);
}

void HUD::move(float x, float y)
{
	position = XMFLOAT2(position.x + x, position.y + y);
}

void HUD::changeText(const wchar_t * newText)
{
	if (isSpriteFont)
	{
		text = newText;
	}
}

