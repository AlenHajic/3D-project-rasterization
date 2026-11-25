#include "MatricesHelper.h"

//Creating worldMatrix
DirectX::XMMATRIX CreateWordlMatrix(const float& angle)
{
    using namespace DirectX;
    XMMATRIX translationMatrix = XMMatrixTranslation(0, 0, -1);
    XMMATRIX rotationMatrix = XMMatrixRotationY(angle);

    return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}

//Creating viewMatrix
DirectX::XMMATRIX CreateViewlMatrix()
{
    using namespace DirectX;
    FXMVECTOR eyePosition = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f); 
    FXMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    FXMVECTOR upPosition = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f); 

    return XMMatrixLookAtLH(eyePosition, focusPosition, upPosition);
}

//Creating projectionMatrix
DirectX::XMMATRIX CreateProjectionMatrix(const float& fovAngleY, const float& aspectRatio, const float& nearZ, const float& farZ)
{
    using namespace DirectX;
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY), aspectRatio, nearZ, farZ);
}

//Creating all matrices
void CreateMatrices(const float& angle, const UINT& WIDTH, const UINT& HEIGHT, DirectX::XMFLOAT4X4*& matrixList)
{
    using namespace DirectX;
    XMMATRIX world = CreateWordlMatrix(angle);
    XMMATRIX view = CreateViewlMatrix();
    float fovAngle = 59.0f;
    float aspectRatio = static_cast<float>(WIDTH)/static_cast<float>(HEIGHT); 
    float nearZ = 0.1f;
    float farZ = 100.0f;
    XMMATRIX projection = CreateProjectionMatrix(fovAngle, aspectRatio, nearZ, farZ);
    XMMATRIX viewProjectionMatrix = XMMatrixMultiplyTranspose(view, projection);

    XMStoreFloat4x4(&matrixList[0], world); 
    XMStoreFloat4x4(&matrixList[1], viewProjectionMatrix);
}

//Updating rotation
void UpdateAngle(float& angle)
{
    using namespace DirectX;
    static auto previousTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - previousTime;
    previousTime = currentTime;
    angle += elapsedTime.count();
}


