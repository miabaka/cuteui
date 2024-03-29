#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <dcomp.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <wrl.h>
#include "glm/vec2.hpp"
#include <ctl/memory.hpp>

#include "cutegfx/Viewport.hpp"
#include "../win32/Win32Window.hpp"

namespace cutegfx {

class Direct3D11Device;

class Direct3D11Viewport : public Viewport {
public:
	explicit Direct3D11Viewport(const ctl::RcPtr<Direct3D11Device> &device);

	void setOutputWindow(const ctl::RcPtr<Window> &window) override;

	void createSwapChain(glm::ivec2 size);

	glm::uvec2 getSize() const override;

	void setSize(glm::uvec2 size) override;

	void clear(glm::u8vec4 color) override;

	void present(bool waitSync) override;

	void use() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> _d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _d3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _d3dRenderTarget;
	Microsoft::WRL::ComPtr<IDCompositionDevice> _compositionDevice;
	Microsoft::WRL::ComPtr<IDCompositionTarget> _compositionTarget;
	Microsoft::WRL::ComPtr<IDCompositionVisual> _compositionVisual;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
	ctl::RcPtr<Direct3D11Device> _device;
	ctl::RcPtr<Win32Window> _outputWindow;
	glm::uvec2 _lastSwapChainSize{};

	void resizeSwapChainIfNecessary(glm::uvec2 newSize);

	void createRenderTarget();
};

} // namespace cutegfx