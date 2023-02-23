#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <memory>

#include <windows.h>
#include <d3d11.h>
#include <dcomp.h>
#include <wrl.h>
#include <cuteutil/SharedObject.hpp>

#include "cutegfx/Device.hpp"
#include "Direct3D11Viewport.hpp"

namespace cutegfx {

class Direct3D11Device : public cuteutil::SharedObject<Direct3D11Device>, public Device {
public:
	Direct3D11Device();

	Direct3D11Device(Direct3D11Device &) = delete;

	Direct3D11Device(Direct3D11Device &&) = delete;

	~Direct3D11Device() override = default;

	Shader::Format getShaderFormat() const override;

	std::shared_ptr<Buffer> createBuffer(Buffer::Type type) override;

	std::shared_ptr<Shader> createShader(Shader::Type type, const void *bytecode, size_t size) override;

	std::shared_ptr<Viewport> createViewport() override;

	void draw(index_t firstIndex, index_t indexCount) override;

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDeviceContext();

	Microsoft::WRL::ComPtr<IDCompositionDevice> getCompositionDevice();

	Microsoft::WRL::ComPtr<IDXGIAdapter> getDxgiAdapter();

	void setActiveViewport(std::weak_ptr<Direct3D11Viewport> viewport);

	bool isViewportActive(const std::shared_ptr<Direct3D11Viewport>& viewport) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
	Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> _dxgiAdapter;
	Microsoft::WRL::ComPtr<IDCompositionDevice> _compositionDevice;
	std::weak_ptr<Direct3D11Viewport> _activeViewport;

	void createDevice();

	void createCompositionDevice();
};

} // namespace cutegfx