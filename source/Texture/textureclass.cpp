////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include <dxstdafx.h>
#include "textureclass.h"

namespace FG
{
	TextureClass::TextureClass()
	{
		m_texture = 0;
	}
	TextureClass::TextureClass(const TextureClass& other)
	{
	}
	TextureClass::~TextureClass()
	{
		Shutdown();
	}
	bool TextureClass::Initialize(ID3D11Device* device, const char* filename)
	{
		HRESULT result;

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = D3DX11_DEFAULT;
		loadInfo.Height = D3DX11_DEFAULT;
		loadInfo.Depth = D3DX11_DEFAULT;
		loadInfo.FirstMipLevel = D3DX11_DEFAULT;
		loadInfo.MipLevels = D3DX11_DEFAULT;
		loadInfo.Usage = D3D11_USAGE_IMMUTABLE;
		loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		loadInfo.CpuAccessFlags = 0;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		loadInfo.Filter = D3DX11_FILTER_SRGB_IN | D3DX11_FILTER_SRGB_OUT | D3DX11_FILTER_NONE;
		loadInfo.MipFilter = D3DX11_DEFAULT;
		loadInfo.pSrcInfo = nullptr;

		// Load the texture in.
		result = D3DX11CreateShaderResourceViewFromFile(device, filename, &loadInfo, nullptr, &m_texture, nullptr);

		if (FAILED(result))
		{
			return false;
		}

		return true;
	}
	void TextureClass::Shutdown()
	{
		// Release the texture resource.
		if (m_texture)
		{
			m_texture->Release();
			m_texture = 0;
		}

		return;
	}
	ID3D11ShaderResourceView* TextureClass::GetTexture() const
	{
		return m_texture;
	}
}