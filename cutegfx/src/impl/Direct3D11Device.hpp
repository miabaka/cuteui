#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <memory>

#include <windows.h>
#include <d3d11.h>
#include <dcomp.h>
#include <wrl.h>
#include <ctl/memory.hpp>

#include "cutegfx/Device.hpp"
#include "Direct3D11Viewport.hpp"

namespace cutegfx {

class Direct3D11Device : public Device {
public:
	Direct3D11Device();

	Shader::Format getShaderFormat() const override;

	ctl::RcPtr<Buffer> createBuffer(Buffer::Type type) override;

	ctl::RcPtr<Shader> createShader(Shader::Type type, const void *bytecode, size_t size) override;

	ctl::RcPtr<Viewport> createViewport() override;

	void draw(index_t firstIndex, index_t indexCount) override;

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDeviceContext();

	Microsoft::WRL::ComPtr<IDCompositionDevice> getCompositionDevice();

	Microsoft::WRL::ComPtr<IDXGIAdapter> getDxgiAdapter();

	void setActiveViewport(const ctl::RcPtr<Direct3D11Viewport> &viewport);

	bool isViewportActive(const ctl::RcPtr<Direct3D11Viewport> &viewport) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
	Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> _dxgiAdapter;
	Microsoft::WRL::ComPtr<IDCompositionDevice> _compositionDevice;
	ctl::WeakRcPtr<Direct3D11Viewport> _activeViewport;

	void createDevice();

	void createCompositionDevice();
};

} // namespace cutegfx