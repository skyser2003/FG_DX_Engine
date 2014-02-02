#include "dxstdafx.h"
#include "PixelShader.h"

namespace FG
{
	void PixelShader::Initialize()
	{
		noCBuffer = 0;
	}
	void PixelShader::Destroy()
	{
		mShader->Release();
		mPixelShaderBuffer->Release();
		mSamplerState->Release();
		for (auto pair : mBufferList)
		{
			pair.second->Release();
		}

		mBufferDescList.clear();
		mBufferList.clear();
	}

	HRESULT PixelShader::CompileShader(const std::string& filename, const std::string& functionName)
	{
		HRESULT result;
		mPixelShaderBuffer = nullptr;
		result = D3DX11CompileFromFile(filename.c_str(), nullptr, NULL, functionName.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &mPixelShaderBuffer, nullptr, nullptr);

		if (FAILED(result))
		{
			return result;
		}

		result = GetDevice()->CreatePixelShader(mPixelShaderBuffer->GetBufferPointer(), mPixelShaderBuffer->GetBufferSize(), nullptr, &mShader);
		if (FAILED(result))
		{
			return result;
		}

		return result;
	}
	HRESULT PixelShader::CreateCBufferDesc(const std::string& bufferName, UINT bufferSize)
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

	HRESULT PixelShader::SetCBufferDesc(const std::string& bufferName, void* bufferData, UINT bufferSize)
	{
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		if (mBufferDescList.find(bufferName) == mBufferDescList.end())
		{
			return false;
		}

		auto bufferDesc = mBufferDescList.find(bufferName)->second;
		auto* buffer = mBufferList.find(bufferName)->second;

		result = GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(result))
		{
			return false;
		}

		memcpy_s(mappedResource.pData, bufferSize, bufferData, bufferSize);

		GetDeviceContext()->Unmap(buffer, 0);
		GetDeviceContext()->PSSetConstantBuffers(noCBuffer++, 1, &buffer);

		return result;
	}
	void PixelShader::EquipShader()
	{
		GetDeviceContext()->PSSetShader(mShader, nullptr, 0);
		GetDeviceContext()->PSSetSamplers(0, 1, &mSamplerState);

	}

	void PixelShader::CreateSamplerState()
	{
		HRESULT result;
		D3D11_SAMPLER_DESC samplerDesc;

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

		result = GetDevice()->CreateSamplerState(&samplerDesc, &mSamplerState);
	}

	void PixelShader::BeginRender()
	{
		noCBuffer = 0;
	}
	void PixelShader::EndRender()
	{
		// À×¿©À×¿©
	}

	void PixelShader::SetTexture(ID3D11ShaderResourceView* texture)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	}
}