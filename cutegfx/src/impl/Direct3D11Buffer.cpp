#include "Direct3D11Buffer.hpp"

#include <cstring>
#include <stdexcept>

using namespace cutegfx;
using Microsoft::WRL::ComPtr;

Direct3D11Buffer::Direct3D11Buffer(std::shared_ptr<Direct3D11Device> device, Buffer::Type type)
		: _device(std::move(device)),
		  _type(type) {
	_d3dDevice = _device->getDevice();
	_d3dContext = _device->getDeviceContext();
}

void Direct3D11Buffer::setData(const void *data, size_t size) {
	if (_size < size)
		createPhysicalBuffer(size);

	D3D11_MAPPED_SUBRESOURCE mapped;
	_d3dContext->Map(_d3dBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	std::memcpy(mapped.pData, data, size);

	_d3dContext->Unmap(_d3dBuffer.Get(), 0);
}

static UINT mapBufferType(Buffer::Type type) {
	switch (type) {
		case Buffer::Type::Index:
			return D3D11_BIND_INDEX_BUFFER;

		case Buffer::Type::Vertex:
			return D3D11_BIND_VERTEX_BUFFER;

		default:
			break;
	}

	return 0;
};

void Direct3D11Buffer::createPhysicalBuffer(size_t size) {
	_size = size;

	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = mapBufferType(_type);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = _d3dDevice->CreateBuffer(&desc, nullptr, &_d3dBuffer);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreateBuffer failed");
}