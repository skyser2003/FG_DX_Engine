#include "dxstdafx.h"
#include "VertexShader.h"


void VertexShader::Initialize()
{
	noCBuffer = 0;
}
void VertexShader::Destroy()
{
}

HRESULT VertexShader::CompileShader(const std::string& filename, const std::string& functionName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;

	// Initialize the pointers this function will use to null.
	errorMessage = NULL;
	mVertexShaderBuffer = nullptr;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(filename.c_str(), nullptr, nullptr, functionName.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr,
		&mVertexShaderBuffer, &errorMessage, nullptr);

	if (FAILED(result))
	{
		/*// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
		OutputShaderErrorMessage(errorMessage, hWnd, functionName);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
		MessageBox(hWnd, functionName.c_str(), "Missing Shader File", MB_OK);
		}

		return false;*/

		return result;
	}

	// Create the vertex shader from the buffer.
	result = GetDevice()->CreateVertexShader(mVertexShaderBuffer->GetBufferPointer(), mVertexShaderBuffer->GetBufferSize(), nullptr, &mShader);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
HRESULT VertexShader::CreateShaderbufferDesc(UINT bufferSize)
{
	if (mShader == nullptr)
	{
		return false;
	}

	HRESULT result;
	ID3D10Blob* errorMessage;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = NULL;

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "COLOR";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "NORMAL";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = GetDevice()->CreateInputLayout(polygonLayout, numElements, mVertexShaderBuffer->GetBufferPointer(), mVertexShaderBuffer->GetBufferSize(),
		&mLayout);
	if (FAILED(result))
	{
		return false;
	}

	mVertexShaderBuffer->Release();
	mVertexShaderBuffer = nullptr;

	return true;
}
HRESULT VertexShader::CreateCBufferDesc(const std::string& bufferName, UINT bufferSize)
{
	HRESULT result;

	if (mShader == nullptr)
	{
		return false;
	}

	if (mBufferDescList.find(bufferName) != mBufferDescList.end())
	{
		return false;
	}

	D3D11_BUFFER_DESC bufferDesc;
	ID3D11Buffer* buffer;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	result = GetDevice()->CreateBuffer(&bufferDesc, nullptr, &buffer);

	if (FAILED(result))
	{
		return result;
	}

	mBufferDescList.insert(std::make_pair(bufferName, bufferDesc));
	mBufferList.insert(std::make_pair(bufferName, buffer));

	return result;
}

HRESULT VertexShader::SetCBufferDesc(const std::string& bufferName, void* bufferData, UINT bufferSize)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (mBufferDescList.find(bufferName) == mBufferDescList.end())
	{
		return false;
	}

	auto bufferDesc = mBufferDescList.find(bufferName)->second;
	auto* buffer = mBufferList.find(bufferName)->second;

	result = GetDeviceContext()->Map(buffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (FAILED(result))
	{
		return result;
	}

	memcpy_s(mappedResource.pData, bufferSize, bufferData, bufferSize);
	GetDeviceContext()->Unmap(buffer, 0);
	GetDeviceContext()->VSSetConstantBuffers(noCBuffer, 1, &buffer);

	++noCBuffer;

	return result;
}
void VertexShader::EquipShader()
{
	GetDeviceContext()->IASetInputLayout(mLayout);
	GetDeviceContext()->VSSetShader(mShader, nullptr, 0);
}

void VertexShader::BeginRender()
{
	noCBuffer = 0;
}
void VertexShader::EndRender()
{
	// À×¿©À×¿©
}

HRESULT VertexShader::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT result;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = GetDevice()->CreateSamplerState(&samplerDesc, &mSampleState);
	if (FAILED(result))
	{
		return false;
	}
}