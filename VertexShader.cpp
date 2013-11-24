#include "dxstdafx.h"
#include "VertexShader.h"


void VertexShader::Initialize()
{
	noCBuffer = 0;
}
void VertexShader::Destroy()
{
	mShader->Release();
	mVertexShaderBuffer->Release();
	mLayout->Release();
	for (auto pair : mBufferList)
	{
		pair.second->Release();
	}

	mBufferDescList.clear();
	mBufferList.clear();
	mShaderBufferDescs.clear();
}

HRESULT VertexShader::CompileShader(const std::string& filename, const std::string& functionName)
{
	HRESULT result;

	// Initialize the pointers this function will use to null.
	mVertexShaderBuffer = nullptr;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(filename.c_str(), nullptr, nullptr, functionName.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr,
		&mVertexShaderBuffer, nullptr, nullptr);

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
	GetDeviceContext()->VSSetConstantBuffers(noCBuffer++, 1, &buffer);

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

void VertexShader::SetupShaderBufferInputType(const std::string& semanticName)
{
	Semantic sem;
	sem.name = semanticName;
	sem.desc.SemanticIndex = 0;
	for (const auto& prevSem : mShaderBufferDescs)
	{
		if (prevSem.name == semanticName)
		{
			++sem.desc.SemanticIndex;
		}
	}

	sem.desc.InputSlot = 0;
	sem.desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	sem.desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	sem.desc.InstanceDataStepRate = 0;

	auto IsSemanticName = [&semanticName](const char* name)
	{
		return semanticName == name;
	};

	if (IsSemanticName("POSITION"))
	{
		sem.desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	}
	else if (IsSemanticName("TEXCOORD"))
	{
		sem.desc.Format = DXGI_FORMAT_R32G32_FLOAT;
	}
	else if (IsSemanticName("COLOR"))
	{
		sem.desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	else if (IsSemanticName("NORMAL"))
	{
		sem.desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	}

	mShaderBufferDescs.push_back(sem);
}

HRESULT VertexShader::CreateShaderBufferDesc()
{
	if (mShader == nullptr)
	{
		return false;
	}

	HRESULT result;
	size_t numElements = mShaderBufferDescs.size();

	D3D11_INPUT_ELEMENT_DESC* layouts = new D3D11_INPUT_ELEMENT_DESC[numElements];

	for (size_t i = 0; i < numElements; ++i)
	{
		mShaderBufferDescs[i].desc.SemanticName = mShaderBufferDescs[i].name.c_str();
		memcpy_s(&layouts[i], sizeof(layouts[i]), &mShaderBufferDescs[i].desc, sizeof(mShaderBufferDescs[i].desc));
	}

	// Create the vertex input layout.
	result = GetDevice()->CreateInputLayout(layouts, static_cast<unsigned>(numElements), mVertexShaderBuffer->GetBufferPointer(), mVertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(result))
	{
		return result;
	}

	delete[] layouts;

	return result;
}