////////////////////////////////////////////////////////////////////////////////
// Filename: fontshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTSHADERCLASS_H_
#define _FONTSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: FontShaderClass
////////////////////////////////////////////////////////////////////////////////
namespace FG
{
	class FontShaderClass
	{
	private:
		struct ConstantBufferType
		{
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
		};

		struct PixelBufferType
		{
			D3DXVECTOR4 pixelColor;
		};

	public:
		FontShaderClass();
		FontShaderClass(const FontShaderClass&);
		~FontShaderClass();

		bool Initialize(ID3D11Device*, HWND);
		void Shutdown();
		bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);

	private:
		bool InitializeShader(ID3D11Device*, HWND, const char*, const char*);
		void ShutdownShader();
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, const char*);

		bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);
		void RenderShader(ID3D11DeviceContext*, int);
	private:
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_constantBuffer;
		ID3D11SamplerState* m_sampleState;
		ID3D11Buffer* m_pixelBuffer;
	};
}
#endif