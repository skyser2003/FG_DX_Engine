#include "dxstdafx.h"
#include "PixelShader.h"

void PixelShader::Initialize()
{
	noCBuffer = 0;
}
void PixelShader::Destroy()
{

}

HRESULT PixelShader::CompileShader(const std::string& filename, const std::string& functionName)
{
	HRESULT result;

	return result;
}
HRESULT PixelShader::CreateShaderbufferDesc(UINT bufferSize)
{
	HRESULT result;

	return result;
}
HRESULT PixelShader::CreateCBufferDesc(const std::string& bufferName, UINT bufferSize)
{
	HRESULT result;

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
}

void PixelShader::SetTexture(ID3D11ShaderResourceView* texture)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
}