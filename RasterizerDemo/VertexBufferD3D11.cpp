#include "VertexBufferD3D11.h"
#include <stdexcept>

VertexBufferD3D11::VertexBufferD3D11(
    ID3D11Device* device,
    UINT sizeOfVertex,
    UINT nrOfVerticesInBuffer,
    void* vertexData)
{
    Initialize(device, sizeOfVertex, nrOfVerticesInBuffer, vertexData);
}

void VertexBufferD3D11:: Initialize(ID3D11Device* device, UINT sizeOfVertex,
    UINT nrOfVerticesInBuffer, void* vertexData)
{
	// Validate parameters
	if (!device || !vertexData || sizeOfVertex == 0 ||
		nrOfVerticesInBuffer == 0)
	{
		throw std::invalid_argument("Invalid vertex buffer parameters");
	}

	if (this->buffer != nullptr)
	{
		throw std::runtime_error("Vertex buffer already initialized");
	}

	//Define buffer desc
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeOfVertex * nrOfVerticesInBuffer;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Define subresource data
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertexData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &this->buffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}

	this->vertexSize = sizeOfVertex;
	this->nrOfVertices = nrOfVerticesInBuffer;
}

VertexBufferD3D11::~VertexBufferD3D11()
{
	if (buffer)
	{
		buffer->Release();
		buffer = nullptr;
	}
}