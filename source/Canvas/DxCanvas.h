#pragma once

#include "RenderInfo.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace FG
{
	class GraphicsClass;
	class ModelClass;

	class VertexShader;
	class PixelShader;

	class TextureManager;

	class DxCanvas
	{
	public:
		DxCanvas();
		~DxCanvas();

		void Initialize(HWND hWnd, int screenWidth, int screenHeight);
		void Destroy();

		void BeginRender();
		void EndRender();
		void Render();

		std::shared_ptr<VertexShader> CreateVertexShader();
		std::shared_ptr<PixelShader> CreatePixelShader();

		void AddRenderInfo(const FG::RenderInfo& info);
		void ClearRenderInfo();

		void EquipVertexShader(const std::shared_ptr<VertexShader>& vs);
		void EquipPixelShader(const std::shared_ptr<PixelShader>& ps);

		HWND GetHwnd() const;
		GraphicsClass* GetGraphics() const;
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;
		TextureManager& GetTextureManager() const;
	private:
		GraphicsClass* mGraphics;
		ModelClass* mModel;

		std::shared_ptr<VertexShader> mVS;
		std::shared_ptr<PixelShader> mPS;

		TextureManager* mTexManager;

		std::vector<FG::RenderInfo> mRenderInfoList;
		std::vector<std::shared_ptr<VertexShader>> mVSList;
		std::vector<std::shared_ptr<PixelShader>> mPSList;

		std::vector<std::shared_ptr<VertexShader>> mEquipVSList;
		std::vector<std::shared_ptr<PixelShader>> mEquipPSList;
	};
}