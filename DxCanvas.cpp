#include "dxstdafx.h"
#include "DxCanvas.h"

#include "graphicsclass.h"
#include "modelclass.h"

#include "PixelShader.h"
#include "VertexShader.h"

#include "TextureManager.h"

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
		auto* position = info.position.data();
		if (info.noVertices != info.position.size()) {
			continue;
		}

		auto* texPosition = info.texPosition.data();
		auto* normal = info.normal.data();

		if (info.noVertices != info.texPosition.size()) {
			texPosition = nullptr;
		}
		if (info.noVertices != info.normal.size()) {
			normal = nullptr;
		}

		mModel->SetRGBA(info.color);
		mModel->SetVertex(mGraphics->GetDevice(), info.noVertices, position, texPosition, normal);
		mModel->Render(mGraphics->GetDeviceContext());
		mGraphics->GetDeviceContext()->DrawIndexed(mModel->GetIndexCount(), 0, 0);

		mModel->Shutdown();
	}

	mRenderInfoList.clear();
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

void DxCanvas::AddRenderInfo(const RenderInfo& info)
{
	mRenderInfoList.push_back(info);
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
