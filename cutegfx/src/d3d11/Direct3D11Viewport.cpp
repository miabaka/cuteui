#include "Direct3D11Viewport.hpp"

#include <cassert>
#include <stdexcept>

#include "glm/gtc/type_ptr.hpp"

#include "Direct3D11Device.hpp"

using namespace cutegfx;
using Microsoft::WRL::ComPtr;

Direct3D11Viewport::Direct3D11Viewport(const ctl::RcPtr<Direct3D11Device> &device)
		: _device(device) {
	_d3dDevice = device->getRawDevice();
	_d3dDeviceContext = device->getRawDeviceContext();
	_compositionDevice = device->getCompositionDevice();

	device->getDxgiAdapter()->GetParent(__uuidof(IDXGIFactory2), &_dxgiFactory);

	_compositionDevice->CreateVisual(&_compositionVisual);
}

void Direct3D11Viewport::setOutputWindow(const ctl::RcPtr<Window> &window) {
	// TODO: properly handle window changing
	assert(window && !_outputWindow);

	_outputWindow = window.dynamicCast<Win32Window>();

	createSwapChain(_outputWindow->getClientSize());
	createRenderTarget();

	_compositionVisual->SetOffsetX(0.f);
	_compositionVisual->SetOffsetY(0.f);
	_compositionVisual->SetContent(_swapChain.Get());

	_compositionDevice->CreateTargetForHwnd(_outputWindow->getHandle(), true, &_compositionTarget);
	_compositionTarget->SetRoot(_compositionVisual.Get());

	_compositionDevice->Commit();
}

void Direct3D11Viewport::createSwapChain(glm::ivec2 size) {
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.Width = size.x;
	swapChainDesc.Height = size.y;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	HRESULT result = _dxgiFactory->CreateSwapChainForComposition(
			_d3dDevice.Get(),
			&swapChainDesc,
			nullptr, // restrictToOutput
			&_swapChain
	);

	if (!SUCCEEDED(result))
		throw std::runtime_error("CreateSwapChainForComposition failed");

	_lastSwapChainSize = size;
}

glm::uvec2 Direct3D11Viewport::getSize() const {
	return _lastSwapChainSize;
}

void Direct3D11Viewport::setSize(glm::uvec2 size) {
	resizeSwapChainIfNecessary(size);
}

void Direct3D11Viewport::clear(glm::u8vec4 color) {
	glm::vec4 floatColor = glm::vec4(color) / 255.f;
	
	floatColor.r *= floatColor.a;
	floatColor.g *= floatColor.a;
	floatColor.b *= floatColor.a;

	_d3dDeviceContext->ClearRenderTargetView(_d3dRenderTarget.Get(), glm::value_ptr(floatColor));
}

// TODO: properly handle minimized windows
void Direct3D11Viewport::present(bool waitSync) {
	_swapChain->Present(waitSync ? 1 : 0, 0);
}

void Direct3D11Viewport::use() {
	_device->setActiveViewport(this);

	D3D11_VIEWPORT viewport{};

	viewport.Width = static_cast<float>(_lastSwapChainSize.x);
	viewport.Height = static_cast<float>(_lastSwapChainSize.y);

	_d3dDeviceContext->RSSetViewports(1, &viewport);

	_d3dDeviceContext->OMSetRenderTargets(1, _d3dRenderTarget.GetAddressOf(), nullptr);
}

void Direct3D11Viewport::resizeSwapChainIfNecessary(glm::uvec2 newSize) {
	if (newSize == _lastSwapChainSize)
		return;

	bool viewportIsActive = _device->isViewportActive(this);

	if (viewportIsActive)
		_d3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	if (_d3dRenderTarget)
		_d3dRenderTarget->Release();

	_swapChain->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0);
	_lastSwapChainSize = newSize;

	if (!viewportIsActive)
		return;

	createRenderTarget();
	use();
}

void Direct3D11Viewport::createRenderTarget() {
	ComPtr<ID3D11Resource> backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, _d3dRenderTarget.GetAddressOf());
}