#include "Direct3D11Viewport.hpp"

#include <cassert>
#include <stdexcept>

#include <wrl.h>

#include "Direct3D11Device.hpp"

using namespace cutegfx;
using Microsoft::WRL::ComPtr;

Direct3D11Viewport::Direct3D11Viewport(std::shared_ptr<Direct3D11Device> device)
		: _device(std::move(device)) {
	_d3dDevice = _device->getDevice();
	_d3dDeviceContext = _device->getDeviceContext();
	_compositionDevice = _device->getCompositionDevice();

	_device->getDxgiAdapter()->GetParent(__uuidof(IDXGIFactory2), &_dxgiFactory);

	_compositionDevice->CreateVisual(&_compositionVisual);
}

void Direct3D11Viewport::setOutputWindow(std::shared_ptr<Window> window) {
	// TODO: properly handle window changing
	assert(window && !_outputWindow);

	_outputWindow = std::dynamic_pointer_cast<Win32Window>(window);

	createSwapChain(_outputWindow->getClientSize());

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

void Direct3D11Viewport::resizeSwapChainIfNecessary(glm::ivec2 newSize) {
	if (newSize == _lastSwapChainSize)
		return;

	_swapChain->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0);
	_lastSwapChainSize = newSize;
}

// TODO: properly handle minimized windows
void Direct3D11Viewport::present(bool waitSync) {
	resizeSwapChainIfNecessary(_outputWindow->getClientSize());

	ComPtr<ID3D11Resource> backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);

	ComPtr<ID3D11RenderTargetView> renderTarget;
	_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);

	float clearColor[] = {0.75f, 0.3f, 1.f, 1.f};
	_d3dDeviceContext->ClearRenderTargetView(renderTarget.Get(), clearColor);

	_swapChain->Present(waitSync ? 1 : 0, 0);
}