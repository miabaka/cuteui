#include "Direct3D11Shader.hpp"

#include <stdexcept>
#include "Direct3D11Device.hpp"

using namespace cutegfx;

Direct3D11Shader::Direct3D11Shader(std::shared_ptr<Direct3D11Device> device, Type type, std::vector<char> &&bytecode)
		: _device(std::move(device)),
		  _bytecode(bytecode),
		  _type(type) {
	_d3dDevice = _device->getDevice();
	_d3dContext = _device->getDeviceContext();

	switch (type) {
		case Type::Pixel:
			createPixelShader();
			break;

		case Type::Vertex:
			createVertexShader();
			break;

		default:
			throw std::invalid_argument("Invalid shader type");
	}
}

Shader::Format Direct3D11Shader::getFormat() const {
	return Format::Hlsl;
}

Shader::Type Direct3D11Shader::getType() const {
	return _type;
}

const void *Direct3D11Shader::getBytecode() const {
	return _bytecode.data();
}

size_t Direct3D11Shader::getBytecodeSize() const {
	return _bytecode.size();
}

void Direct3D11Shader::use() {
	switch (_type) {
		case Type::Pixel:
			_d3dContext->PSSetShader(_d3dPixelShader.Get(), nullptr, 0);
			break;

		case Type::Vertex:
			_d3dContext->VSSetShader(_d3dVertexShader.Get(), nullptr, 0);
			break;
	}
}

void Direct3D11Shader::createVertexShader() {
	HRESULT result = _d3dDevice->CreateVertexShader(
			_bytecode.data(),
			_bytecode.size(),
			nullptr, // classLinkage
			&_d3dVertexShader
	);

	if (FAILED(result))
		throw std::runtime_error("CreateVertexShader failed");
}

void Direct3D11Shader::createPixelShader() {
	HRESULT result = _d3dDevice->CreatePixelShader(
			_bytecode.data(),
			_bytecode.size(),
			nullptr, // classLinkage
			&_d3dPixelShader
	);

	if (FAILED(result))
		throw std::runtime_error("CreatePixelShader failed");
}