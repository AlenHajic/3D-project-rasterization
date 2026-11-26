#include "ConstantBufferD3D11.h"

ConstantBufferD3D11::ConstantBufferD3D11(ID3D11Device* device, size_t byteSize, void* initialData)
{
	Initialize(device, byteSize, initialData);
}

ConstantBufferD3D11::~ConstantBufferD3D11()
{
	if (buffer)
	{
		buffer->Release();
		buffer = nullptr;
	}
}

ConstantBufferD3D11::ConstantBufferD3D11(ConstantBufferD3D11&& other) noexcept
{
	this->buffer = other.buffer;
	this->bufferSize = other.bufferSize;
	other.buffer = nullptr;
	other.bufferSize = 0;
}

ConstantBufferD3D11& ConstantBufferD3D11::operator=(ConstantBufferD3D11&& other) noexcept
{
	//No self assignment
	if (this != &other)
	{
		//Release current exisiting
		if (this->buffer)
		{
			this->buffer->Release();
		}

		this->buffer = other.buffer;
		this->bufferSize = other.bufferSize;

		//reset other
		other.buffer = nullptr;
		other.bufferSize = 0;
	}

	return *this;
}

void ConstantBufferD3D11::Initialize(ID3D11Device* device, size_t byteSize, void* initialData)
{
	if (!device || byteSize == 0 || byteSize % 16 != 0)
	{
		throw std::invalid_argument("Invalid constant buffer parameters");
	}

	if (this->buffer != nullptr)
	{
		throw std::runtime_error("Constant buffer already initialized");
	}

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(byteSize);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = initialData;

	HRESULT hr;

	if (initialData)
	{
		hr = device->CreateBuffer(&bufferDesc, &data, &buffer);
	}
	else
	{
		hr = device->CreateBuffer(&bufferDesc, nullptr, &buffer);
	}

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer");
	}

	this->bufferSize = byteSize;
}

size_t ConstantBufferD3D11::GetSize() const
{
	return this->bufferSize;
}

ID3D11Buffer* ConstantBufferD3D11::GetBuffer() const
{
	return this->buffer;
}

void ConstantBufferD3D11::UpdateBuffer(ID3D11DeviceContext* context, void* data)
{
	if (!context || !data || !buffer)
	{
		throw std::invalid_argument("Invalid parameters for updating constant buffer");
	}

	//map
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to map constant buffer");
	}

	//COpy our new data
	std::memcpy(mappedResource.pData, data, bufferSize);

	//Unmap buffer
	context->Unmap(buffer, 0);
}
