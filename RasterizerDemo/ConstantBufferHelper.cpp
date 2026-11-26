#include "ConstantBufferHelper.h"
#include "WindowHelper.h"

//Function for creating CBuffer for vertex shader
bool CreateVSConstantBuffer(ID3D11Device* device, ConstantBufferD3D11& constantBuffer, DirectX::XMFLOAT4X4 matrixList[2])
{
	//Removed junk like desc and "creating buffer" since our init does these things
	constantBuffer.Initialize(device, sizeof(DirectX::XMFLOAT4X4) * 2, matrixList); // *2 for 2 matrices?
	return true;
}

//Function for creating Cbuffer for pixel shader
bool CreatePSConstantBuffer(ID3D11Device* device, ConstantBufferD3D11& constantBuffer)
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

	constantBuffer.Initialize(device, sizeof(lightProperties), &lightProperties);

	return true;
}

//Setup for the buffers
bool SetupConstantBuffer(ID3D11Device* device, ConstantBufferD3D11& constantVSBuffer, ConstantBufferD3D11& constantPSBuffer, const UINT WIDTH, const UINT HEIGHT, const float& angle, DirectX::XMFLOAT4X4 matrixList[2])
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
