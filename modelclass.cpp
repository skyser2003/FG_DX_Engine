#include <dxstdafx.h>

#include <graphicsclass.h>
#include <modelclass.h>

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	vertices = nullptr;

	r = g = b = a = 1.0f;
	rotationX = rotationY = rotationZ = 0.0f;
}
ModelClass::ModelClass(const ModelClass& other)
{
}
ModelClass::~ModelClass()
{
}

void ModelClass::SetR(float r)
{
	this->r = r;
}
void ModelClass::SetG(float g)
{
	this->g = g;
}
void ModelClass::SetB(float b)
{
	this->b = b;
}
void ModelClass::SetA(float a)
{
	this->a = a;
}
void ModelClass::SetRGBA(float r,float g,float b,float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void ModelClass::SetVertex(ID3D11Device* device, int noVertices, const D3DXVECTOR3* positions, const D3DXVECTOR2* texPositions, const D3DXVECTOR3* normal)
{
	VertexType* vertices = new VertexType[noVertices];
	unsigned long* indices = new unsigned long[noVertices];

	memset(vertices, 0, sizeof(VertexType)* noVertices);

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = noVertices;

	// Set the number of indices in the index array.
	m_indexCount = noVertices;

	// Load the index array with data.
	// Counter-Clockwise
	for (int i = 0; i < m_indexCount; ++i)
	{
		indices[i] = i;
	}

	for(int i=0;i<m_vertexCount;++i)
	{
		vertices[i].position = positions[i];
		vertices[i].color = D3DXVECTOR4(r,g,b,a);
	}

	if (texPositions != nullptr)
	{
		for (int i = 0; i < m_vertexCount; ++i)
		{
			vertices[i].texture = texPositions[i];
		}
	}
	if (normal != nullptr)
	{
		for (int i = 0; i < m_vertexCount; ++i)
		{
			vertices[i].normal = normal[i];
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices[0]) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices[0]) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	delete[] vertices;
	delete[] indices;
}
void ModelClass::SetRotation(float x, float y, float z)
{
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

void ModelClass::InitializeRect(ID3D11Device* device,float x,float y,float width,float height,
								float r,float g,float b,float a,float vectorX,float vectorY)
{
	//Wrong direction, set to default
	if(vectorX == 0.0f && vectorY == 0.0f)
		vectorY = 1.0f;

	const int n = 2;

	unsigned long* indices = new unsigned long[3 * n];

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 3 * n;

	// Set the number of indices in the index array.
	m_indexCount = 3 * n;

	float hw = width/2;
	float hh = height/2;

	float PI = atan(1.0f) * 4.0f;

	float rotationRadius = sqrt(pow(vectorX,2) + pow(vectorY,2));
	float rotationAngle;

	if(vectorY == 0)
	{
		int signX = vectorX >=0 ? 1 : -1;
		rotationAngle = PI/2.0f * (-1) * signX;
	}
	else if(vectorX == 0)
	{
		if(vectorY < 0)
			rotationAngle = PI;
		else
			rotationAngle = 0.0f;
	}
	else
	{
		rotationAngle = atan(vectorX/vectorY);
	}

	VertexType* vertices = new VertexType[m_vertexCount];
	float** positions = new float*[m_vertexCount];
	for(int i=0;i<m_vertexCount;++i)
		positions[i] = new float[2];

	positions[0][0] = -hw;
	positions[0][1] = hh;

	positions[1][0] = hw;
	positions[1][1] = hh;
	
	positions[2][0] = hw;
	positions[2][1] = -hh;
	
	positions[3][0] = hw;
	positions[3][1] = -hh;
	
	positions[4][0] = -hw;
	positions[4][1] = -hh;
	
	positions[5][0] = -hw;
	positions[5][1] = hh;

	for(int i=0;i<3 * n;++i)
	{
		//Matrix rotation
		float oldPosition0 = positions[i][0];
		positions[i][0] = cos(rotationAngle) * positions[i][0] - sin(rotationAngle) * positions[i][1];
		positions[i][1] = sin(rotationAngle) * oldPosition0 + cos(rotationAngle) * positions[i][1];

		vertices[i].position = D3DXVECTOR3(x + positions[i][0],y +positions[i][1],0.0f);
		vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}

	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[0].color = D3DXVECTOR4(r,g,b,a);

	vertices[1].texture = D3DXVECTOR2(1.0f, 0.0f);
	vertices[1].color = D3DXVECTOR4(r,g,b,a);

	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].color = D3DXVECTOR4(r,g,b,a);

	vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[3].color = D3DXVECTOR4(r,g,b,a);

	vertices[4].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[4].color = D3DXVECTOR4(r,g,b,a);

	vertices[5].texture = D3DXVECTOR2(0.0f, 0.0f);
	vertices[5].color = D3DXVECTOR4(r,g,b,a);

	// Load the index array with data.
	// Clockwise
	for(int i=0;i<m_indexCount;++i)
		indices[i] = i;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices[0]) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices[0]) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	for(int i=0;i<m_vertexCount;++i)
		delete[] positions[i];

	delete[] positions;
	delete[] vertices;
	delete[] indices;
}
void ModelClass::InitializeCircle(ID3D11Device* device,float x,float y,float radius,
								  float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f)
{
	int n = 3;
	float PI = atan(1.0f) * 4.0f;
	
	unsigned long* indices = new unsigned long[3*n];
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 3 * n;

	// Set the number of indices in the index array.
	m_indexCount = 3 * n;

	VertexType* vertices = new VertexType[m_vertexCount];

	for(int i=0;i<3*n;i+=3)
	{
		float angle = 2*PI / n * (i/3);
		float angle2 = 2*PI / n * (i/3 + 1);

		vertices[i].position = D3DXVECTOR3(x,y,0);
		vertices[i].texture = D3DXVECTOR2(0.5f,0.5f);
		vertices[i].color = D3DXVECTOR4(r,g,b,a);

		vertices[i+1].position = D3DXVECTOR3(x + radius * std::cos(angle),y + radius * std::sin(angle),0);
		vertices[i+1].texture = D3DXVECTOR2(0.5f + 0.5f * std::cos(angle),0.5f - 0.5f * std::sin(angle));
		vertices[i+1].color = D3DXVECTOR4(r,g,b,a);

		vertices[i+2].position = D3DXVECTOR3(x + radius * std::cos(angle2),y + radius * std::sin(angle2),0);
		vertices[i+2].texture = D3DXVECTOR2(0.5f + 0.5f * std::cos(angle2),0.5f - 0.5f * std::sin(angle2));
		vertices[i+2].color = D3DXVECTOR4(r,g,b,a);
	}

	for(int i=0;i<3*n;++i)
	{
		vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}

	// Load the index array with data.
	// Clockwise
	for(int i=0;i<m_indexCount;++i)
		indices[i] = i;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices[0]) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices[0]) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	delete[] vertices;
	delete[] indices;
}

void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}
int ModelClass::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11Buffer* ModelClass::GetVertexBuffer() const
{
	return m_vertexBuffer;
}
ID3D11Buffer* ModelClass::GetIndexBuffer() const
{
	return m_indexBuffer;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}