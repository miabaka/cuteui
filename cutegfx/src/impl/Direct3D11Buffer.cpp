#include "Direct3D11Buffer.hpp"

#include <cstring>
#include <stdexcept>

#include "Direct3D11Shader.hpp"

using namespace cutegfx;
using Microsoft::WRL::ComPtr;

Direct3D11Buffer::Direct3D11Buffer(std::shared_ptr<Direct3D11Device> device, Buffer::Type type)
		: _device(std::move(device)),
		  _type(type) {
	_d3dDevice = _device->getDevice();
	_d3dContext = _device->getDeviceContext();
}

static DXGI_FORMAT mapElementTypeToDxgiFormat(Buffer::ElementType type) {
	switch (type) {
		case Buffer::ElementType::Float2:
			return DXGI_FORMAT_R32G32_FLOAT;

		case Buffer::ElementType::Float4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		default:
			return DXGI_FORMAT_UNKNOWN;
	}
}

static size_t mapElementTypeToSize(Buffer::ElementType type) {
	switch (type) {
		case Buffer::ElementType::Float2:
			return sizeof(float) * 2;

		case Buffer::ElementType::Float4:
			return sizeof(float) * 4;

		default:
			return 0;
	}
}

void Direct3D11Buffer::setLayout(const std::vector<ElementDesc> &elements, std::shared_ptr<Shader> vertexShader) {
	if (vertexShader->getType() != Shader::Type::Vertex)
		throw std::invalid_argument("Shader must be vertex");

	std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs;

	elementDescs.reserve(elements.size());

	D3D11_INPUT_ELEMENT_DESC elementDesc{};

	elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	_stride = 0;

	for (auto &element: elements) {
		elementDesc.SemanticName = element.name;
		elementDesc.Format = mapElementTypeToDxgiFormat(element.type);

		elementDescs.emplace_back(elementDesc);

		_stride += mapElementTypeToSize(element.type);
	}

	HRESULT result = _d3dDevice->CreateInputLayout(
			elementDescs.data(),
			elementDescs.size(),
			vertexShader->getBytecode(),
			vertexShader->getBytecodeSize(),
			&_d3dInputLayout
	);

	if (FAILED(result))
		throw std::runtime_error("CreateInputLayout failed");
}

void Direct3D11Buffer::setData(const void *data, size_t size) {
	if (size < 1)
		return;
	else if (size > _size)
		createPhysicalBuffer(size);

	D3D11_MAPPED_SUBRESOURCE mapped;
	_d3dContext->Map(_d3dBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	std::memcpy(mapped.pData, data, size);

	_d3dContext->Unmap(_d3dBuffer.Get(), 0);
}

void Direct3D11Buffer::use() {
	UINT stride = static_cast<UINT>(_stride);
	UINT offset = 0;

	switch (_type) {
		case Type::Constant:
			_d3dContext->VSSetConstantBuffers(0, 1, _d3dBuffer.GetAddressOf());
			_d3dContext->PSSetConstantBuffers(0, 1, _d3dBuffer.GetAddressOf());
			break;

		case Type::Index:
			_d3dContext->IASetIndexBuffer(_d3dBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			break;

		case Type::Vertex:
			_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			_d3dContext->IASetVertexBuffers(0, 1, _d3dBuffer.GetAddressOf(), &stride, &offset);
			_d3dContext->IASetInputLayout(_d3dInputLayout.Get());
			break;
	}
}

static UINT mapBufferType(Buffer::Type type) {
	switch (type) {
		case Buffer::Type::Index:
			return D3D11_BIND_INDEX_BUFFER;

		case Buffer::Type::Vertex:
			return D3D11_BIND_VERTEX_BUFFER;

		case Buffer::Type::Constant:
			return D3D11_BIND_CONSTANT_BUFFER;

		default:
			return 0;
	}
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