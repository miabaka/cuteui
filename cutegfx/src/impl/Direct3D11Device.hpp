#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <d3d11.h>
#include <dcomp.h>
#include <wrl.h>
#include <cuteutil/SharedObject.hpp>

#include "cutegfx/Device.hpp"

namespace cutegfx {

class Direct3D11Device : public cuteutil::SharedObject<Direct3D11Device>, public Device {
public:
	Direct3D11Device();

	Direct3D11Device(Direct3D11Device &) = delete;

	Direct3D11Device(Direct3D11Device &&) = delete;

	~Direct3D11Device() override = default;

	std::shared_ptr<Buffer> createBuffer(Buffer::Type type) override;

	std::shared_ptr<Viewport> createViewport() override;

	Microsoft::WRL::ComPtr<ID3D11Device> getDevice();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDeviceContext();

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

} // namespace cutegfx