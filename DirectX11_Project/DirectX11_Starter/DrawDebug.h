#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include "DirectXGameCore.h"
#include "Camera.h"
#include "SimpleShader.h"
#include <d3d11.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"


using namespace DirectX; 

class DrawDebug : public btIDebugDraw
{
	int m_debugMode; 
public:
	DrawDebug(ID3D11Device* dev,
		ID3D11DeviceContext* con, 
		Camera* cam, 
		SimpleVertexShader* vs,
		SimplePixelShader* ps);
	~DrawDebug();

	void Update();
	void CopyToGPU();
	void Draw(); 
	XMFLOAT3 convertBtVec(const btVector3& vec); 

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	virtual void	drawSphere(const btVector3& p, btScalar radius, const btVector3& color);

	virtual void	drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha);

	virtual void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void	reportErrorWarning(const char* warningString);

	virtual void	draw3dText(const btVector3& location, const char* textString);

	virtual void	setDebugMode(int debugMode);

	virtual int		getDebugMode() const { return m_debugMode; }

private: 
	vector<XMFLOAT3> vertices;
	SimpleVertexShader* vertexShader; 
	SimplePixelShader* pixelShader;
	ID3D11DeviceContext* context; 
	Camera* camera;
	ID3D11Device * device;
	XMFLOAT4X4 worldMatrix; 
	ID3D11Buffer* vertexBuffer;
	XMFLOAT4X4 position; 
	XMFLOAT4X4 rotation; 
	XMFLOAT4X4 scalar; 



};

