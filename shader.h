#pragma once

class Shader
{
public:
	virtual ~Shader() {}

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

	virtual HRESULT CompileShader(const std::string& filename, const std::string& functionName) = 0;
	virtual HRESULT CreateCBufferDesc(const std::string& bufferName, UINT bufferSize) = 0;
	
	virtual HRESULT SetCBufferDesc(const std::string& bufferName, void* bufferData, UINT bufferSize) = 0;
	virtual void EquipShader() = 0;

	void SetDevice(ID3D11Device* device)
	{
		mDevice = device;
	}
	void SetDeviceContext(ID3D11DeviceContext* devCon)
	{
		mDevCon = devCon;
	}

	ID3D11Device* GetDevice() const
	{
		return mDevice;
	}
	ID3D11DeviceContext* GetDeviceContext() const
	{
		return mDevCon;
	}
private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDevCon;
};