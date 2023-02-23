#pragma once

#include <memory>

#include <d3d11.h>
#include <wrl.h>

#include "cutegfx/Buffer.hpp"
#include "Direct3D11Device.hpp"

namespace cutegfx {

class Direct3D11Buffer : public Buffer {
public:
	explicit Direct3D11Buffer(std::shared_ptr<Direct3D11Device> device, Buffer::Type type);

	void setLayout(const std::vector<ElementDesc> &elements, std::shared_ptr<Shader> vertexShader) override;

	void setData(const void *data, size_t size) override;

	void use() override;

private:
	Type _type;
	std::shared_ptr<Direct3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11Device> _d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _d3dContext;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _d3dBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _d3dInputLayout;
	size_t _size = 0;
	size_t _stride = 0;

	void createPhysicalBuffer(size_t size);
};

} // namespace cutegfx