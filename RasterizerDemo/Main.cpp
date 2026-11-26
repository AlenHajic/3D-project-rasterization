#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
#include <chrono>

#include "WindowHelper.h"
#include "D3D11Helper.h"
#include "PipelineHelper.h"
#include "ConstantBufferHelper.h"
#include "MatricesHelper.h"
#include "VertexBufferD3D11.h"

#define _CRTDBG_MAP_ALLOC

using namespace DirectX;

void Render(ID3D11DeviceContext* immediateContext, ID3D11RenderTargetView* rtv,
			ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader,
			ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, const VertexBufferD3D11& vertexBuffer,
	       ID3D11ShaderResourceView* srv, ID3D11SamplerState* samplerState)
{
	float clearColour[4] = { 0, 0, 0, 0 };
	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = vertexBuffer.GetVertexSize();
	UINT offset = 0;

	ID3D11Buffer* VertexBuffer = vertexBuffer.GetBuffer();

	immediateContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	immediateContext->VSSetShader(vShader, nullptr, 0);

	immediateContext->RSSetViewports(1, &viewport);

	immediateContext->PSSetShader(pShader, nullptr, 0);

	immediateContext->PSSetShaderResources(0, 1, &srv);

	immediateContext->PSSetSamplers(0, 1, &samplerState);

	immediateContext->OMSetRenderTargets(1, &rtv, dsView);

	immediateContext->Draw(vertexBuffer.GetNrOfVertices(), 0);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;
	HWND window;

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Failed to setup window!" << std::endl;
		return -1;
	}

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;

	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* rtv;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* samplerState;

	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;

	D3D11_VIEWPORT viewport;

	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;

	ID3D11InputLayout* inputLayout;

	VertexBufferD3D11 vertexBuffer;
	ID3D11Buffer* VSConstBuffer = nullptr;
	ID3D11Buffer* PSConstBuffer = nullptr;

	//setting up d3d11
	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to setup d3d11!" << std::endl;
		return -1;
	}

	//setting up various things lmao
	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout,
		texture, srv, samplerState))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	//setting up constantbuffers
	float angle = 0.0f;	
	XMFLOAT4X4 matrixList[2]{};
	if (!SetupConstantBuffer(device, VSConstBuffer, PSConstBuffer, WIDTH, HEIGHT, angle,matrixList))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	immediateContext->VSSetConstantBuffers(0, 1, &VSConstBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &PSConstBuffer);

	MSG msg = { };
	
	//Window looper
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	
		if (GetKeyState('A') & 0x8000)
		{
			angle += 0.0001f;
		}

		if (GetKeyState('D') & 0x8000)
		{
			angle += -0.0001f;
		}

		UpdateAngle(angle);

		//To hold mapped memory
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		immediateContext->Map(VSConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		DirectX::XMStoreFloat4x4(&matrixList[0], CreateWordlMatrix(angle));
		memcpy(mappedResource.pData, matrixList, sizeof(matrixList));
		immediateContext->Unmap(VSConstBuffer, 0);
		
		Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, srv, samplerState);
		swapChain->Present(0, 0);
	}

	/*vertexBuffer.~VertexBufferD3D11();*/  //destuctor calls itself automatically
	VSConstBuffer->Release();
	PSConstBuffer->Release();
	inputLayout->Release();
	pShader->Release();
	vShader->Release();
	dsView->Release();
	dsTexture->Release();
	texture->Release();
	srv->Release();
	samplerState->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}