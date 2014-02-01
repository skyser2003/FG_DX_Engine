#include "dxstdafx.h"
#include "DxCanvas.h"

#include "graphicsclass.h"
#include "modelclass.h"

#include "PixelShader.h"
#include "VertexShader.h"

#include "TextureManager.h"
#include "textureclass.h"

namespace FG
{
	DxCanvas::DxCanvas()
	{
	}
	DxCanvas::~DxCanvas()
	{
	}

	void DxCanvas::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		mGraphics = new GraphicsClass;
		mModel = new ModelClass;
		mTexManager = new TextureManager;

		mGraphics->Initialize(screenWidth, screenHeight, hWnd);
		mTexManager->SetDevice(mGraphics->GetDevice());
	}
	void DxCanvas::Destroy()
	{
		mModel->Shutdown();
		mGraphics->Shutdown();

		mRenderInfoList.clear();

		for (auto vs : mVSList)	{
			vs->Destroy();
		}
		for (auto ps : mPSList)	{
			ps->Destroy();
		}

		delete mModel;
		delete mGraphics;
	}

	void DxCanvas::BeginRender()
	{
		mGraphics->InitDraw();
		for (auto& vs : mVSList) {
			vs->BeginRender();
		}
		for (auto& ps : mPSList) {
			ps->BeginRender();
		}
	}
	void DxCanvas::EndRender()
	{
		for (auto& ps : mPSList) {
			ps->EndRender();
		}
		for (auto& vs : mVSList) {
			vs->EndRender();
		}
		mGraphics->EndDraw();
	}

	void DxCanvas::Render()
	{
		mVS->EquipShader();
		mPS->EquipShader();

		for (const auto& info : mRenderInfoList)
		{
			if (info.texture != nullptr) {
				mPS->SetTexture(info.texture->GetTexture());
			}
			mModel->SetRGBA(D3DXVECTOR4(1, 1, 1, 1));
			mModel->SetVertex(mGraphics->GetDevice(), info.noVertices, info.buffer, info.bufferSize);
			mModel->Render(mGraphics->GetDeviceContext());
			mGraphics->GetDeviceContext()->DrawIndexed(mModel->GetIndexCount(), 0, 0);

			mModel->Shutdown();
		}

		ClearRenderInfo();
	}

	std::shared_ptr<VertexShader> DxCanvas::CreateVertexShader()
	{
		std::shared_ptr<VertexShader> vs(new VertexShader);
		mVSList.push_back(vs);

		vs->Initialize();
		vs->SetDevice(mGraphics->GetDevice());
		vs->SetDeviceContext(mGraphics->GetDeviceContext());

		return vs;
	}
	std::shared_ptr<PixelShader> DxCanvas::CreatePixelShader()
	{
		std::shared_ptr<PixelShader> ps(new PixelShader);
		mPSList.push_back(ps);

		ps->Initialize();
		ps->SetDevice(mGraphics->GetDevice());
		ps->SetDeviceContext(mGraphics->GetDeviceContext());

		return ps;
	}

	void DxCanvas::AddRenderInfo(const FG::RenderInfo& info)
	{
		mRenderInfoList.push_back(info);
	}
	void DxCanvas::ClearRenderInfo()
	{
		for (auto& info : mRenderInfoList)
		{
			delete info.buffer;
		}

		mRenderInfoList.clear();
	}

	void DxCanvas::EquipVertexShader(const std::shared_ptr<VertexShader>& vs)
	{
		mVS = vs;
	}
	void DxCanvas::EquipPixelShader(const std::shared_ptr<PixelShader>& ps)
	{
		mPS = ps;
	}

	HWND DxCanvas::GetHwnd() const
	{
		return mGraphics->GetHwnd();
	}
	GraphicsClass* DxCanvas::GetGraphics() const
	{
		return mGraphics;
	}
	ID3D11Device* DxCanvas::GetDevice() const
	{
		return mGraphics->GetDevice();
	}
	ID3D11DeviceContext* DxCanvas::GetDeviceContext() const
	{
		return mGraphics->GetDeviceContext();
	}
}