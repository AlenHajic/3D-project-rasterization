#include <fstream>
#include <string>
#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include "PipelineHelper.h"
#include "WindowHelper.h"
#include "stb_image.h"
#include "VertexBufferD3D11.h"

using namespace std;

//Load shaders? Check a little more
bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open VS file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}

	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	reader.open("PixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open PS file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
		return false;
	}

	return true;
}

//Load textures
bool LoadTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv, const char* filename)
{
	int width, height, channels; 
	unsigned char* imageData = stbi_load(filename, &width, &height, &channels, 4);
	if (!imageData)
	{
		cerr << "Failed to load texture" << filename << endl;
	}

	//Texture description
	D3D11_TEXTURE2D_DESC textureDesc
	{
		textureDesc.Width = width,
		textureDesc.Height = height,
		textureDesc.MipLevels = 1, 
		textureDesc.ArraySize = 1, 
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM, 
		textureDesc.SampleDesc = {1, 0}, 
		textureDesc.Usage = D3D11_USAGE_IMMUTABLE, 
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE,
		//Dont need to modify CPU accses since we already did it under "usage"
	};

	//Subresrouce data
	D3D11_SUBRESOURCE_DATA textureSubData
	{
		textureSubData.pSysMem = imageData, //Pointer to the our image data
		textureSubData.SysMemPitch = width * 4, 
		textureSubData.SysMemSlicePitch = 0
	};
	
	//We create our texture
	HRESULT hr = device->CreateTexture2D(&textureDesc, &textureSubData, &texture); //Creates 2D texture on the GPU
	if(FAILED(hr))
	{
		cerr << "Failed to create texture!" << endl;
		stbi_image_free(imageData);
		return false;
	}

	//We create our shaderresource view
	hr = device->CreateShaderResourceView(texture, nullptr, &srv);
	if(FAILED(hr))
	{
		cerr << "Failed to create shaderresource view!" << endl;
		texture->Release();
		stbi_image_free(imageData);
		return false;
	}
	stbi_image_free(imageData);

	return true;
}

//Create our samplerstate
bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState)
{
	//Sampler description
	D3D11_SAMPLER_DESC samplerDesc
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
		samplerDesc.MipLODBias = 0,
		samplerDesc.MaxAnisotropy = 1, //check this
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER, //Is this for shadows? MAybe have to turn on later?
		samplerDesc.BorderColor[0] = 0,
		samplerDesc.BorderColor[1] = 0,
		samplerDesc.BorderColor[2] = 0,
		samplerDesc.BorderColor[3] = 0,
		samplerDesc.MinLOD = 0,
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX

	};

	//Create samplerstate
	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	return !(FAILED(hr));
}

//Create inputlayout
bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode)
{
	//Input layout description
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//Create inputlayout
	HRESULT hr = device->CreateInputLayout(inputDesc, 3, vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);

	return !FAILED(hr);
}

//Create vertex buffer
bool CreateVertexBuffer(ID3D11Device* device, VertexBufferD3D11& vertexBuffer)
{
	//Clockwise by default in D3D11
	SimpleVertex quad[] =
	{
		{ {-0.5, 0.5f, 0.0f}, {0, 0, -1}, {0, 0}},
		{ {0.5, 0.5f, 0.0f}, {0, 0, -1}, {1, 0}},
		{ {-0.5, -0.5f, 0.0f}, {0, 0, -1}, {0, 1}},
		{ {0.5, -0.5f, 0.0f}, {0, 0, -1}, {1, 1}},
	};

	vertexBuffer.Initialize(device, sizeof(SimpleVertex), 4, quad);

	return true;
}

//Setup for everything
bool SetupPipeline(ID3D11Device* device, VertexBufferD3D11& vertexBuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout,
	               ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv, ID3D11SamplerState*& samplerState)
{
	std::string vShaderByteCode;
	//Load Shader
	if (!LoadShaders(device, vShader, pShader, vShaderByteCode))
	{
		std::cerr << "Error loading shaders!" << std::endl;
		return false;
	}

	//Load/Create texture
	if (!LoadTexture(device, texture, srv, "HEHE.png"))
	{
		std::cerr << "Error loading shaders!" << std::endl;
		return false;
	}

	//Create our samplerstate
	if (!CreateSamplerState(device, samplerState))
	{
		std::cerr << "Error creating samplerstate!" << endl;
		return false;
	}

	//Create input layout
	if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}

	//Create vertex buffer
	if (!CreateVertexBuffer(device, vertexBuffer))
	{
		std::cerr << "Error creating vertex buffer!" << std::endl;
		return false;
	}

	return true;
}




