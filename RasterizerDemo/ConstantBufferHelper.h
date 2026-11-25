#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "MatricesHelper.h"
#include <iostream>

bool CreateVSConstantBuffer(ID3D11Device* device, ID3D11Buffer*& constantBuffer, DirectX::XMFLOAT4X4 matrixList[2]); //ConstantBuffer for the vertex shader/transformations, rotations, 
																													 //scale etc
bool CreatePSConstantBuffer(ID3D11Device* device, ID3D11Buffer*& constantBuffer);
bool SetupConstantBuffer(ID3D11Device* device, ID3D11Buffer*& constantBuffer, ID3D11Buffer*& constantPSBuffer, const UINT WIDTH, const UINT HEIGHT, const float& angle, DirectX::XMFLOAT4X4 matrixList[2]);