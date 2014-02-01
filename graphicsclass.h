////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////

namespace FG
{
	class D3DClass;
	class CameraClass;
	class ModelClass;
	class TextureShaderClass;
	class TextClass;

	/////////////
	// GLOBALS //
	/////////////
	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 0.1f;//1000.0f
	const float SCREEN_NEAR = -0.1f;//0.1f


	////////////////////////////////////////////////////////////////////////////////
	// Class name: GraphicsClass
	////////////////////////////////////////////////////////////////////////////////
	class GraphicsClass
	{
	public:
		GraphicsClass();
		GraphicsClass(const GraphicsClass&);
		virtual ~GraphicsClass();

		virtual bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
		virtual void Shutdown();

		virtual void InitDraw();
		void EndDraw();

		void SetCameraX(float x);
		void SetCameraY(float y);
		void SetCameraZ(float z);
		void SetCameraPosition(float x, float y, float z);

		void SetRotationX(float x);
		void SetRotationY(float y);
		void SetRotationZ(float z);
		void SetRotation(float x, float y, float z);

		D3DClass* GetD3D() const { return  m_D3D; }
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;
		HWND GetHwnd() const { return hwnd; }

		D3DXMATRIX GetWorldMatrix() const { return worldMatrix; }
		D3DXMATRIX GetViewMatrix() const { return viewMatrix; }
		D3DXMATRIX GetProjectionMatrix() const { return projectionMatrix; }

		D3DXVECTOR3 GetCameraPosition() const;
		D3DXVECTOR3 GetRotation() const;
	protected:
		D3DClass* m_D3D;
		CameraClass* camera;
		D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
		HWND hwnd;
		float rotationX, rotationY, rotationZ;

		int m_indexCount;
	};
}