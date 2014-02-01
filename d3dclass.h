////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
namespace FG
{
	class D3DClass
	{
	public:
		D3DClass();
		D3DClass(const D3DClass&);
		~D3DClass();

		bool Initialize(int, int, bool, HWND, bool, float, float);
		void Shutdown();

		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		void BeginScene(float, float, float, float);
		void EndScene();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void GetProjectionMatrix(D3DXMATRIX&);
		void GetWorldMatrix(D3DXMATRIX&);
		void GetOrthoMatrix(D3DXMATRIX&);

		void GetVideoCardInfo(char*, int&);

	private:
		bool m_vsync_enabled;
		int m_videoCardMemory;
		char m_videoCardDescription[128];
		IDXGISwapChain* m_swapChain;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState* m_rasterState;
		D3DXMATRIX m_projectionMatrix;
		D3DXMATRIX m_worldMatrix;
		D3DXMATRIX m_orthoMatrix;
		ID3D11DepthStencilState* m_depthDisabledStencilState;
		ID3D11BlendState* m_alphaEnableBlendingState;
		ID3D11BlendState* m_alphaDisableBlendingState;
	};
}
#endif