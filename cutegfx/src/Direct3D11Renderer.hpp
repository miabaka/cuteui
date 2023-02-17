#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <d3d11.h>
#include <dcomp.h>
#include <wrl.h>
#include <cuteutil/SharedObject.hpp>

#include "cutegfx/Renderer.hpp"

class Direct3D11Renderer : public cuteutil::SharedObject<Direct3D11Renderer>, public Renderer {
public:
	Direct3D11Renderer();

	Direct3D11Renderer(Direct3D11Renderer &) = delete;

	Direct3D11Renderer(Direct3D11Renderer &&) = delete;

	~Direct3D11Renderer() override = default;

	std::shared_ptr<Viewport> createViewport() override;

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice();

	Microsoft::WRL::ComPtr<IDCompositionDevice> getCompositionDevice();

	Microsoft::WRL::ComPtr<IDXGIAdapter> getDxgiAdapter();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
	Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> _dxgiAdapter;
	Microsoft::WRL::ComPtr<IDCompositionDevice> _compositionDevice;

	void createDevice();

	void createCompositionDevice();
};