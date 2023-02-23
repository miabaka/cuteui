#include "Direct3D11Device.hpp"

#include <stdexcept>

#include "Direct3D11Buffer.hpp"
#include "Direct3D11Shader.hpp"
#include "Direct3D11Viewport.hpp"

using namespace cutegfx;
using Microsoft::WRL::ComPtr;

Direct3D11Device::Direct3D11Device() {
	createDevice();
	createCompositionDevice();
}

Shader::Format Direct3D11Device::getShaderFormat() const {
	return Shader::Format::Hlsl;
}

std::shared_ptr<Buffer> Direct3D11Device::createBuffer(Buffer::Type type) {
	return std::make_shared<Direct3D11Buffer>(asShared(), type);
}

std::shared_ptr<Shader> Direct3D11Device::createShader(Shader::Type type, const void *bytecode, size_t size) {
	auto *bytecodeBegin = static_cast<const char *>(bytecode);
	auto *bytecodeEnd = bytecodeBegin + size;

	std::vector<char> bytecodeVec(bytecodeBegin, bytecodeEnd);

	return std::make_shared<Direct3D11Shader>(asShared(), type, std::move(bytecodeVec));
}

std::shared_ptr<Viewport> Direct3D11Device::createViewport() {
	return Direct3D11Viewport::create(asShared());
}

void Direct3D11Device::draw(index_t firstIndex, index_t indexCount) {
	_deviceContext->DrawIndexed(indexCount, firstIndex, 0);
}

ComPtr<ID3D11Device> Direct3D11Device::getDevice() {
	return _device;
}

ComPtr<ID3D11DeviceContext> Direct3D11Device::getDeviceContext() {
	return _deviceContext;
}

ComPtr<IDCompositionDevice> Direct3D11Device::getCompositionDevice() {
	return _compositionDevice;
}

ComPtr<IDXGIAdapter> Direct3D11Device::getDxgiAdapter() {
	return _dxgiAdapter;
}

void Direct3D11Device::setActiveViewport(std::weak_ptr<Direct3D11Viewport> viewport) {
	_activeViewport = std::move(viewport);
}

bool Direct3D11Device::isViewportActive(const std::shared_ptr<Direct3D11Viewport> &viewport) const {
	return viewport == _activeViewport.lock();
}

void Direct3D11Device::createDevice() {
	HRESULT result = D3D11CreateDevice(
			nullptr, // adapter
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, // software
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr, 0, // featureLevels
			D3D11_SDK_VERSION,
			&_device,
			nullptr, // featureLevel
			&_deviceContext
	);

	if (!SUCCEEDED(result))
		throw std::runtime_error("D3D11CreateDevice failed");

	_device->QueryInterface(__uuidof(IDXGIDevice), &_dxgiDevice);
	_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &_dxgiAdapter);
}

void Direct3D11Device::createCompositionDevice() {
	HRESULT result = DCompositionCreateDevice(_dxgiDevice.Get(), __uuidof(IDCompositionDevice), &_compositionDevice);

	if (!SUCCEEDED(result))
		throw std::runtime_error("DCompositionCreateDevice failed");
}