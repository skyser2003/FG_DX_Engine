#include <dxstdafx.h>

#include "Graphics/graphicsclass.h"
#include "Model/modelclass.h"

namespace FG
{
	ModelClass::ModelClass()
	{
		m_vertexBuffer = 0;
		m_indexBuffer = 0;

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
	void ModelClass::SetRGBA(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	void ModelClass::SetRGBA(D3DXVECTOR4 rgba)
	{
		r = rgba[0];
		g = rgba[1];
		b = rgba[2];
		a = rgba[3];
	}
		
	void ModelClass::SetVertex(ID3D11Device* device, int noVertices, void* buffer, int bufferSize)
	{
		unsigned long* indices = new unsigned long[noVertices];

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		// Set the number of indices in the index array.
		m_vertexCount = noVertices;
		m_indexCount = noVertices;

		// Load the index array with data.
		// Counter-Clockwise
		for (int i = 0; i < m_indexCount; ++i)
		{
			indices[i] = i;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = bufferSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = buffer;
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

		delete[] indices;
	}
	void ModelClass::SetRotation(float x, float y, float z)
	{
		rotationX = x;
		rotationY = y;
		rotationZ = z;
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
		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = 0;
		}

		// Release the vertex buffer.
		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = 0;
		}

		return;
	}
	void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
	{
		D3D11_BUFFER_DESC desc;
		m_vertexBuffer->GetDesc(&desc);

		UINT* stride = new UINT[m_vertexCount];
		UINT* offset = new UINT[m_vertexCount];

		// Set vertex buffer stride and offset.
		for (int i = 0; i < m_vertexCount; ++i)
		{
			stride[i] = desc.ByteWidth / m_vertexCount;
			offset[i] = 0;
		}

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, stride, offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		delete[] stride;
		delete[] offset;

		return;
	}
}