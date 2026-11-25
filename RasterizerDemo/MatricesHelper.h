#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <chrono>

#include "MatricesHelper.h"

DirectX::XMMATRIX CreateWordlMatrix(const float& angle);
DirectX::XMMATRIX CreateViewlMatrix();
DirectX::XMMATRIX CreateProjectionMatrix(const float& fovAngleY, const float& aspectRatio, const float& nearZ, const float& farZ);
void CreateMatrices(const float& angle, const UINT& WIDTH, const UINT& HEIGHT, DirectX::XMFLOAT4X4*& matrixArray); 
void UpdateAngle(float& angle);
