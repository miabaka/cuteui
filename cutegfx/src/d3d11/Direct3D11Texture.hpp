#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <ctl/memory.hpp>

#include "cutegfx/Device.hpp"
#include "cutegfx/Texture.hpp"

namespace cutegfx {

class Direct3D11Device;

class Direct3D11Texture : public Texture {
public:
	explicit Direct3D11Texture(
			const ctl::RcPtr<Direct3D11Device> &device, glm::uvec2 size = {}, const void *data = nullptr);

private:
	ctl::RcPtr<Direct3D11Device> _device;

	Microsoft::WRL::ComPtr<ID3D11Device> _rawDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _rawContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _rawTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _rawResourceView;

	void use() override;

	void createTexture(glm::uvec2 size, const void *data);

	void createResourceView();
};

} // namespace cutegfx