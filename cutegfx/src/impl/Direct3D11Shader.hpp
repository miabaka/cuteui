#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <memory>
#include <vector>

#include <windows.h>
#include <wrl.h>
#include <d3d11.h>

#include "cutegfx/Shader.hpp"

namespace cutegfx {

class Direct3D11Device;

class Direct3D11Shader : public Shader {
public:
	explicit Direct3D11Shader(std::shared_ptr<Direct3D11Device> device, Type type, std::vector<char> &&bytecode);

	Format getFormat() const override;

	Type getType() const override;

	const void *getBytecode() const override;

	size_t getBytecodeSize() const override;

	void use() override;

private:
	std::shared_ptr<Direct3D11Device> _device;
	std::vector<char> _bytecode;
	Type _type;
	Microsoft::WRL::ComPtr<ID3D11Device> _d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _d3dContext;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _d3dPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _d3dVertexShader;

	void createVertexShader();

	void createPixelShader();
};

} // namespace cutegfx