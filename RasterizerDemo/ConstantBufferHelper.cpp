#include "ConstantBufferHelper.h"
#include "WindowHelper.h"

//Function for creating CBuffer for vertex shader
bool CreateVSConstantBuffer(ID3D11Device* device, ID3D11Buffer*& constantBuffer, DirectX::XMFLOAT4X4 matrixList[2])
{
	//Buffer description
	D3D11_BUFFER_DESC bufferDesc = {
		bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4[2]), 
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC, //GPU can read from this and CPU can write 
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE //Needs to be able to write on CPU
	};

	//subresources
	D3D11_SUBRESOURCE_DATA data = {
		data.pSysMem = matrixList,
		data.SysMemPitch = 0,
		data.SysMemSlicePitch = 0,
	};
	
	//Create constantBuffer
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &constantBuffer);
	if (FAILED(hr))
	{
		std::cerr << "Error creating constant buffer! (VS)" << std::endl;
		return false;
	}

	return true;
}

//Function for creating Cbuffer for pixel shader
bool CreatePSConstantBuffer(ID3D11Device* device, ID3D11Buffer*& constantBuffer)
{
	//Struct for creating our light
	struct lightProperties
	{
		DirectX::XMFLOAT4 lightPosition = { 0.0f, 0.5f, -5.0f, 1.0f };
		DirectX::XMFLOAT4 lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4 cameraPosition = { 0.0f, 0.0f, -3.0f, 0.0f };
		float ambientLightIntensity = 0.01f;
		float shininess = 200.0f;
		float padding[2] = {}; //Need to keep the struct a multiple of 16 for some reason? Not sure why but oki lol
	} lightProperties;

	//Buffer description
	D3D11_BUFFER_DESC bufferDesc = {
		bufferDesc.ByteWidth = sizeof(lightProperties),
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE,
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER
	};

	//Buffer subresources
	D3D11_SUBRESOURCE_DATA data = {
		data.pSysMem = &lightProperties,
		data.SysMemPitch = 0,
		data.SysMemSlicePitch = 0
	};

	//Create buffer 
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &constantBuffer);
	if (FAILED(hr))
	{
		std::cerr << "Error creating constant buffer! (PS)" << std::endl;
		return false;
	}

	return true;
}

//Setup for the buffers
bool SetupConstantBuffer(ID3D11Device* device, ID3D11Buffer*& constantVSBuffer, ID3D11Buffer*& constantPSBuffer, const UINT WIDTH, const UINT HEIGHT, const float& angle, DirectX::XMFLOAT4X4 matrixList[2])
{
	//Creating all my matrices
    CreateMatrices(angle, WIDTH, HEIGHT, matrixList);

	//Checking if we can create constant buffer for VS
	if (!CreateVSConstantBuffer(device, constantVSBuffer, matrixList))
	{
		std::cerr << "Error creating constant buffer! (VS)" << std::endl;
		return false;
	}

	//Checking if we can create constant buffer for PS
	if (!CreatePSConstantBuffer(device, constantPSBuffer))
	{
		std::cerr << "Error creating constant buffer! (PS)" << std::endl;
		return false;
	}

    return true;
}
