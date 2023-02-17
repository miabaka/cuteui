#include "Direct3D11Renderer.hpp"

#include <stdexcept>
#include "Direct3D11Viewport.hpp"

using Microsoft::WRL::ComPtr;

Direct3D11Renderer::Direct3D11Renderer() {
	createDevice();
	createCompositionDevice();
}

std::shared_ptr<Viewport> Direct3D11Renderer::createViewport() {
	return std::make_shared<Direct3D11Viewport>(asShared());
}

ComPtr<ID3D11Device> Direct3D11Renderer::getDevice() {
	return _device;
}

ComPtr<ID3D11DeviceContext> Direct3D11Renderer::getDeviceContext() {
	return _deviceContext;
}

ComPtr<IDCompositionDevice> Direct3D11Renderer::getCompositionDevice() {
	return _compositionDevice;
}

ComPtr<IDXGIAdapter> Direct3D11Renderer::getDxgiAdapter() {
	return _dxgiAdapter;
}

void Direct3D11Renderer::createDevice() {
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

void Direct3D11Renderer::createCompositionDevice() {
	HRESULT result = DCompositionCreateDevice(_dxgiDevice.Get(), __uuidof(IDCompositionDevice), &_compositionDevice);

	if (!SUCCEEDED(result))
		throw std::runtime_error("DCompositionCreateDevice failed");
}