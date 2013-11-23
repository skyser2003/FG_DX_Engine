#include <dxstdafx.h>
#include <graphicsclass.h>

#include <d3dclass.h>
#include <cameraclass.h>
#include <textclass.h>
#include <modelclass.h>

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	camera = nullptr;
}
GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}
GraphicsClass::~GraphicsClass()
{
}
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	this->hwnd = hwnd;

	bool result;
	
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = new CameraClass;
	if(!camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	camera->SetPosition(screenWidth / 2.0f, screenHeight / 2.0f, -1.0f * screenHeight / 2.0f);
	camera->Render();
	camera->GetViewMatrix(viewMatrix);

	rotationX = rotationY = rotationZ = 0.0f;
	
	return true;
}
void GraphicsClass::Shutdown()
{
	// Release the camera object.
	if(camera)
	{
		delete camera;
		camera = nullptr;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}

	return;
}

void GraphicsClass::InitDraw()
{
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationYawPitchRoll(&worldMatrix, rotationY, rotationX, rotationZ);
}
void GraphicsClass::EndDraw()
{
	m_D3D->EndScene();
}

void GraphicsClass::SetCameraX(float x)
{
	auto pos = camera->GetPosition();
	camera->SetPosition(x, pos.y, pos.z);
}
void GraphicsClass::SetCameraY(float y)
{
	auto pos = camera->GetPosition();
	camera->SetPosition(pos.x, y, pos.z);
}
void GraphicsClass::SetCameraZ(float z)
{
	auto pos = camera->GetPosition();
	camera->SetPosition(pos.x, pos.y, z);
}
void GraphicsClass::SetCameraPosition(float x, float y, float z)
{
	camera->SetPosition(x, y, z);
}

void GraphicsClass::SetRotationX(float x)
{
	rotationX = x;
}
void GraphicsClass::SetRotationY(float y)
{
	rotationY = y;
}
void GraphicsClass::SetRotationZ(float z)
{
	rotationZ = z;
}
void GraphicsClass::SetRotation(float x, float y, float z)
{
	SetRotationX(x);
	SetRotationY(y);
	SetRotationZ(z);
}

ID3D11Device* GraphicsClass::GetDevice() const
{
	return m_D3D->GetDevice();
}
ID3D11DeviceContext* GraphicsClass::GetDeviceContext() const
{
	return m_D3D->GetDeviceContext();
}

D3DXVECTOR3 GraphicsClass::GetCameraPosition() const
{
	return camera->GetPosition();
}
D3DXVECTOR3 GraphicsClass::GetRotation() const
{
	return D3DXVECTOR3(rotationX, rotationY, rotationZ);
}