#include "DrawDebug.h"



#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
// --------------------------------------------------------
struct LineVertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
};


DrawDebug::DrawDebug(ID3D11Device * dev, ID3D11DeviceContext* con,
	Camera* cam, SimpleVertexShader * vs, SimplePixelShader * ps)
{
	device = dev; 
	vertexShader = vs; 
	pixelShader = ps; 
	context = con; 
	camera = cam; 
	/*for (int i = 0; i < points.size(); i++)
	{
		vertices.push_back(XMFLOAT3(points[i].getX(), points[i].getY(), points[i].getZ())); 
		cout << points[i].getX() << " " << points[i].getY() << " " << points[i].getZ() << endl; 
	}*/

	// Create buffers for drawing lines

	//// DYNAMIC vertex buffer (no initial data necessary)
	D3D11_BUFFER_DESC vbDesc = {};
	//ZeroMemory(&vbDesc, sizeof(vbDesc));
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	//vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//vbDesc.CPUAccessFlags = 0; 
	
	//vbDesc.ByteWidth = sizeof(LineVertex) * vertices.size();
	//Allocate room for 500 debug lines 
	vbDesc.ByteWidth = sizeof(LineVertex) * 1000;


	device->CreateBuffer(&vbDesc, 0, &vertexBuffer);

	//Ignore 
	//Initialize matrices 
	position = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0,
		0.0f, 1.0f, 0.0f, 0,
		0.0f, 0.0f, 1.0f, 0,
		0, 0, 0, 1.0f);
	rotation = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	scalar = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	worldMatrix = XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	
}

DrawDebug::~DrawDebug()
{
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Release();
	}
	
}

void DrawDebug::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & fromColor, const btVector3 & toColor)
{
	XMFLOAT3 dxFrom = convertBtVec(from); 
	XMFLOAT3 dxTo = convertBtVec(to);
	XMFLOAT3 dxFromColor = convertBtVec(fromColor); 
	XMFLOAT3 dxToColor = convertBtVec(toColor); 
	//pixelShader->SetData("inColor", &dxFromColor, sizeof(XMFLOAT3));
	vertices.push_back(dxFrom); 
	vertices.push_back(dxTo); 

	// DYNAMIC vertex buffer (no initial data necessary)
	//D3D11_BUFFER_DESC vbDesc = {};
	////ZeroMemory(&vbDesc, sizeof(vbDesc));
	//vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	////vbDesc.Usage = D3D11_USAGE_DEFAULT;
	//vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	////vbDesc.CPUAccessFlags = 0; 

	////vbDesc.ByteWidth = sizeof(LineVertex) * vertices.size();
	//vbDesc.ByteWidth = sizeof(LineVertex);


	//device->CreateBuffer(&vbDesc, 0, &vertexBuffer);

}

void DrawDebug::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	XMFLOAT3 dxFrom = convertBtVec(from);
	XMFLOAT3 dxTo = convertBtVec(to);
	XMFLOAT3 dxColor = convertBtVec(color); 
	pixelShader->SetData("inColor", &dxColor, sizeof(XMFLOAT3));
	vertices.push_back(dxFrom);
	vertices.push_back(dxTo);
}

void DrawDebug::drawSphere(const btVector3 & p, btScalar radius, const btVector3 & color)
{
	int sliceCount = 20; 
	int stackCount = 20; 
}

void DrawDebug::drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha)
{
}

void DrawDebug::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
}

void DrawDebug::reportErrorWarning(const char * warningString)
{
}

void DrawDebug::draw3dText(const btVector3 & location, const char * textString)
{
}

void DrawDebug::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void DrawDebug::Update()
{
	XMMATRIX worldMat = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX translation = XMLoadFloat4x4(&position);
	XMMATRIX rot = XMLoadFloat4x4(&rotation);
	XMMATRIX scaling = XMLoadFloat4x4(&scalar);

	worldMat = translation * rot * scaling;

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(worldMat));
}

XMFLOAT3 DrawDebug::convertBtVec(const btVector3& vec)
{
	return XMFLOAT3(vec.getX(), vec.getY(), vec.getZ());
}

void DrawDebug::CopyToGPU()
{
	// All lines copied locally - send whole buffer to GPU
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HRESULT hr = context->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	memcpy(mapped.pData, &vertices[0], sizeof(LineVertex) * vertices.size());

	context->Unmap(vertexBuffer, 0);
}

void DrawDebug::Draw()
{
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// Copy to dynamic buffer
	CopyToGPU();

	// Set up buffers
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	//vertexShader->SetMatrix4x4("")
	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", camera->getViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera->getProjectionMatrix());
	vertexShader->SetShader();

	//pixelShader->SetShaderResourceView("particle", XMFLOAT4(1,0);
	pixelShader->SetData("inColor", &XMFLOAT3(1.0f, 0.0f, 0.0f), sizeof(XMFLOAT3));
	pixelShader->SetShader();

	context->Draw(vertices.size(), 0);
	

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Clear out vertices
	vertices.clear(); 
	//Make functions that add and remove lines 
}