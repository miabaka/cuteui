#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <dcomp.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <wrl.h>
#include "glm/vec2.hpp"

#include "cutegfx/Viewport.hpp"
#include "Win32PlatformWindow.hpp"

class Direct3D11Renderer;

class Direct3D11Viewport : public Viewport {
public:
	explicit Direct3D11Viewport(std::shared_ptr<Direct3D11Renderer> renderer);

	void setOutputWindow(std::shared_ptr<PlatformWindow> window) override;

	void createSwapChain(glm::ivec2 size);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
	Microsoft::WRL::ComPtr<IDCompositionDevice> _compositionDevice;
	Microsoft::WRL::ComPtr<IDCompositionTarget> _compositionTarget;
	Microsoft::WRL::ComPtr<IDCompositionVisual> _compositionVisual;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
	std::shared_ptr<Direct3D11Renderer> _renderer;
	std::shared_ptr<Win32PlatformWindow> _outputWindow;
};