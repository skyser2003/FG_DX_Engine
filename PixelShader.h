#pragma once

#include "shader.h"

class PixelShader : public Shader
{
public:
	PixelShader() {}
	virtual ~PixelShader() {}

	virtual void Initialize();
	virtual void Destroy();

	virtual HRESULT CompileShader(const std::string& filename, const std::string& functionName) override;
	virtual HRESULT CreateCBufferDesc(const std::string& bufferName, UINT bufferSize) override;

	virtual HRESULT SetCBufferDesc(const std::string& bufferName, void* bufferData, UINT bufferSize) override;
	virtual void EquipShader() override;

	void CreateSamplerState();

	void BeginRender();
	void EndRender();

	void SetTexture(ID3D11ShaderResourceView* texture);

private:
	ID3D11PixelShader* mShader;
	ID3D10Blob* mPixelShaderBuffer;
	ID3D11SamplerState* mSamplerState;

	std::map<std::string, D3D11_BUFFER_DESC> mBufferDescList;
	std::map<std::string, ID3D11Buffer*> mBufferList;
	int noCBuffer;
};