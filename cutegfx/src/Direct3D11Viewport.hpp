#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <dcomp.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <wrl.h>
#include <glm/vec2.hpp>

#include "cutegfx/Viewport.hpp"
#include "Win32Window.hpp"

namespace cutegfx {

class Direct3D11Device;

class Direct3D11Viewport : public Viewport {
public:
	explicit Direct3D11Viewport(std::shared_ptr<Direct3D11Device> device);

	void setOutputWindow(std::shared_ptr<Window> window) override;

	void createSwapChain(glm::ivec2 size);

	void clear(glm::vec4 color) override;

	void resize(glm::uvec2 size) override;

	void present(bool waitSync) override;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> _d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _d3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGIFactory2> _dxgiFactory;
	Microsoft::WRL::ComPtr<IDCompositionDevice> _compositionDevice;
	Microsoft::WRL::ComPtr<IDCompositionTarget> _compositionTarget;
	Microsoft::WRL::ComPtr<IDCompositionVisual> _compositionVisual;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
	std::shared_ptr<Direct3D11Device> _device;
	std::shared_ptr<Win32Window> _outputWindow;
	glm::uvec2 _lastSwapChainSize{};

	void resizeSwapChainIfNecessary(glm::uvec2 newSize);
};

} // namespace cutegfx