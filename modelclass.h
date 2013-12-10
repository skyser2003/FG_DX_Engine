////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////
// MY CLASS INCLUDES //
///////////////////////

class GraphicsClass;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 color;
		D3DXVECTOR3 normal;
	};
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	
	void SetR(float r); 
	void SetG(float g);
	void SetB(float b);
	void SetA(float a);
	void SetRGBA(float r, float g, float b, float a);
	void SetRGBA(D3DXVECTOR4 rgba);
	void SetVertex(ID3D11Device* device,int noVertices,const D3DXVECTOR3* positions, const D3DXVECTOR2* texPositions = nullptr, const D3DXVECTOR3* normal = nullptr);
	void SetRotation(float x, float y, float z);

	void InitializeRect(ID3D11Device* device,float x,float y,float width,float height,
		float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
		float vectorX = 0.0f,float vectorY = 1.0f);
	void InitializeCircle(ID3D11Device* device,float x,float y,float radius,float r,float g,float b,float a);
	void InitializeCircle(ID3D11Device* device,float x,float y,int vectorX,int vectorY,float radius,float r,float g,float b,float a);

	void Render(ID3D11DeviceContext*);
	void Shutdown();
	
	int GetIndexCount() const;

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
private:
	float r,g,b,a;
	float rotationX, rotationY, rotationZ;
	D3DXMATRIX rotation;
	VertexType* vertices;

	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void Rotate(float centerX,float centerY,float halfWidth,float halfHeight,float rotationAngle);

	//std::unique_ptr<ID3D11Buffer> m_vertexBuffer,m_indexBuffer;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};