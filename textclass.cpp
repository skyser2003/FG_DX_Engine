///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include <dxstdafx.h>
#include "textclass.h"
namespace FG
{
	float TextClass::relativeX;
	float TextClass::relativeY;

	TextClass::TextClass()
	{
		m_Font = 0;

		m_sentence = 0;
	}
	TextClass::TextClass(const TextClass& other)
	{
	}
	TextClass::~TextClass()
	{
	}

	void TextClass::SetRelativeX(float x)
	{
		relativeX = x;
	}
	void TextClass::SetRelativeY(float y)
	{
		relativeY = y;
	}
	void TextClass::SetRelativePosition(float x, float y)
	{
		relativeX = x;
		relativeY = y;
	}

	bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight,
		D3DXMATRIX baseViewMatrix, const char* fontFile)
	{
		bool result;

		// Store the screen width and height.
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		// Store the base view matrix.
		m_baseViewMatrix = baseViewMatrix;

		// Create the font object.
		m_Font = new FontClass;
		if (!m_Font)
		{
			return false;
		}

		// Initialize the font object.
		result = m_Font->Initialize(device, fontFile);
		if (!result)
		{
			MessageBox(hwnd, "Could not initialize the font object.", "Error", MB_OK);
			return false;
		}

		relativeX = 0.0f;
		relativeY = 0.0f;

		return true;
	}
	void TextClass::Shutdown()
	{
		// Release the first sentence.
		ReleaseSentence();

		// Release the font object.
		if (m_Font)
		{
			m_Font->Shutdown();
			delete m_Font;
			m_Font = 0;
		}

		return;
	}
	bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix, TextureClass* texture)
	{
		bool result;

		// Draw the first sentence.
		result = RenderSentence(deviceContext, m_sentence, worldMatrix, orthoMatrix, texture);
		if (!result)
		{
			return false;
		}

		return true;
	}
	int TextClass::GetIndexCount()
	{
		return m_sentence->indexCount;
	}

	bool TextClass::InitializeSentence(int maxLength, ID3D11Device* device)
	{
		VertexType* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		int i;


		// Create a new sentence object.
		m_sentence = new SentenceType;
		if (!m_sentence)
		{
			return false;
		}

		// Initialize the sentence buffers to null.
		(m_sentence)->vertexBuffer = 0;
		(m_sentence)->indexBuffer = 0;

		// Set the maximum length of the sentence.
		(m_sentence)->maxLength = maxLength;

		// Set the number of vertices in the vertex array.
		(m_sentence)->vertexCount = 6 * maxLength;

		// Set the number of indexes in the index array.
		(m_sentence)->indexCount = (m_sentence)->vertexCount;

		// Create the vertex array.
		vertices = new VertexType[(m_sentence)->vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[(m_sentence)->indexCount];
		if (!indices)
		{
			return false;
		}

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(VertexType)* (m_sentence)->vertexCount));

		// Initialize the index array.
		for (i = 0; i < (m_sentence)->indexCount; i++)
		{
			indices[i] = i;
		}

		// Set up the description of the dynamic vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType)* (m_sentence)->vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(m_sentence)->vertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long)* (m_sentence)->indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &(m_sentence)->indexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Release the vertex array as it is no longer needed.
		delete[] vertices;
		vertices = 0;

		// Release the index array as it is no longer needed.
		delete[] indices;
		indices = 0;

		return true;
	}
	bool TextClass::UpdateSentence(char* text, int positionX, int positionY, float red, float green, float blue, float alpha,
		ID3D11DeviceContext* deviceContext, float fontSize)
	{
		positionX += relativeX;
		positionY += relativeY;

		int numLetters;
		VertexType* vertices;
		float drawX, drawY;
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexType* verticesPtr;


		// Store the color of the m_sentence.
		m_sentence->red = red;
		m_sentence->green = green;
		m_sentence->blue = blue;

		// Get the number of letters in the m_sentence.
		numLetters = (int)strlen(text);

		// Check for possible buffer overflow.
		if (numLetters > m_sentence->maxLength)
		{
			return false;
		}

		// Create the vertex array.
		vertices = new VertexType[m_sentence->vertexCount];
		if (!vertices)
		{
			return false;
		}

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(VertexType)* m_sentence->vertexCount));

		// Calculate the X and Y pixel position on the screen to start drawing to.
		drawX = (float)positionX;
		drawY = (float)positionY;

		// Use the font class to build the vertex array from the m_sentence text and m_sentence draw location.
		m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY, red, green, blue, alpha, fontSize);

		// Lock the vertex buffer so it can be written to.
		result = deviceContext->Map(m_sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		// Get a pointer to the data in the vertex buffer.
		verticesPtr = (VertexType*)mappedResource.pData;

		// Copy the data into the vertex buffer.
		memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)* m_sentence->vertexCount));

		// Unlock the vertex buffer.
		deviceContext->Unmap(m_sentence->vertexBuffer, 0);

		// Release the vertex array as it is no longer needed.
		delete[] vertices;
		vertices = 0;

		return true;
	}
	void TextClass::ReleaseSentence()
	{
		if (m_sentence)
		{
			// Release the sentence vertex buffer.
			if ((m_sentence)->vertexBuffer)
			{
				(m_sentence)->vertexBuffer->Release();
				(m_sentence)->vertexBuffer = 0;
			}

			// Release the sentence index buffer.
			if ((m_sentence)->indexBuffer)
			{
				(m_sentence)->indexBuffer->Release();
				(m_sentence)->indexBuffer = 0;
			}

			// Release the sentence.
			delete m_sentence;
			m_sentence = 0;
		}

		return;
	}
	bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix, TextureClass* texture)
	{
		unsigned int stride, offset;

		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		return true;
	}
}