#pragma once

#include "shader.h"

struct SampleVertexBuffer
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR4 color;
	D3DXVECTOR3 normal;
};

struct MatrixBufferType
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

struct LightBufferType
{
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float calculateLight;
};

class VertexShader : public Shader
{
public:
	VertexShader() {}
	virtual ~VertexShader() {}

	virtual void Initialize();
	virtual void Destroy();

	virtual HRESULT CompileShader(const std::string& filename, const std::string& functionName) override;
	virtual HRESULT CreateCBufferDesc(const std::string& bufferName, UINT bufferSize) override;
	
	virtual HRESULT SetCBufferDesc(const std::string& bufferName, void* bufferData, UINT bufferSize) override;
	virtual void EquipShader() override;

	void BeginRender();
	void EndRender();

	virtual HRESULT CreateShaderbufferDesc();

	ID3D11VertexShader* GetShader() const { return mShader; }
private:
	ID3D11VertexShader* mShader;
	ID3D10Blob* mVertexShaderBuffer;
	ID3D11InputLayout* mLayout;
	ID3D11SamplerState* mSampleState;

	std::map<std::string, D3D11_BUFFER_DESC> mBufferDescList;
	std::map<std::string, ID3D11Buffer*> mBufferList;
	int noCBuffer;
};