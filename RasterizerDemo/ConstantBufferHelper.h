#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "MatricesHelper.h"
#include <iostream>
#include "ConstantBufferD3D11.h"

bool CreateVSConstantBuffer(ID3D11Device* device, ConstantBufferD3D11& constantBuffer, DirectX::XMFLOAT4X4 matrixList[2]); //ConstantBuffer for the vertex shader/transformations, rotations, 
																													 //scale etc
bool CreatePSConstantBuffer(ID3D11Device* device, ConstantBufferD3D11& constantBuffer);
bool SetupConstantBuffer(ID3D11Device* device, ConstantBufferD3D11& constantBuffer, ConstantBufferD3D11& constantPSBuffer, const UINT WIDTH, const UINT HEIGHT, const float& angle, DirectX::XMFLOAT4X4 matrixList[2]);