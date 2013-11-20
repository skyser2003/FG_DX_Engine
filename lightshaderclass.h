#pragma once

class LightShaderClass
{
public:
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
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND, const char* vsFilename, const char* psFilename, const char* vsFuncName, const char* psFuncName);
	void Shutdonwn();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, LightBufferType lightBuffer);
private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, const char* vsFilename, const char* psFilename, const char* vsFuncName, const char* psFuncName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const char*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, LightBufferType lightBuffer);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* samplerState;
	ID3D11Buffer* matrixBuffer;
	
	ID3D11Buffer* lightBuffer;
};