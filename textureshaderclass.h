////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&);
	TextureShaderClass(HWND hWnd);
	~TextureShaderClass();

	bool Initialize(ID3D11Device* device, HWND hWnd);
	bool SetVertexShader(ID3D11Device* device, const char* filename, const char* functionName);
	bool SetPixelShader(ID3D11Device* device, const char* filename, const char* functionName);
	void Shutdown();
	bool Render(ID3D11DeviceContext* devcon, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
		D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture = nullptr);

	void SetHWND(HWND hWnd) { this->hWnd = hWnd;}

	ID3D11InputLayout* GetInputLayout() const;
	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;
	ID3D11SamplerState* GetSamplerState() const;

private:
 	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const char* message);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	HWND hWnd;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_transparentBuffer;
};

#endif