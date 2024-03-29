#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <cstdint>

#include <windows.h>
#include <d3d11.h>
#include <dcomp.h>
#include <wrl.h>
#include "ctl/memory.hpp"

#include "cutegfx/Device.hpp"
#include "Direct3D11Texture.hpp"
#include "Direct3D11Viewport.hpp"

namespace cutegfx {

class Direct3D11Device : public Device {
public:
#pragma pack(push, 4)
	struct ConstantBufferData {
		glm::mat4 projection;
		bool useTexture;
		char pad[15];
	};
#pragma pack(pop)

	Direct3D11Device();

	ctl::RcPtr<Viewport> createViewport() override;

	ctl::RcPtr<Texture> createTexture() override;

	ctl::RcPtr<Texture> createTexture(glm::uvec2 size, const void *data) override;

	void setMesh(const InputMesh &mesh) override;

	void draw(InputMesh::index_t firstIndex, InputMesh::index_t indexCount) override;

	Microsoft::WRL::ComPtr<ID3D11Device> getRawDevice();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> getRawDeviceContext();

	Microsoft::WRL::ComPtr<IDCompositionDevice> getCompositionDevice();

	Microsoft::WRL::ComPtr<IDXGIAdapter> getDxgiAdapter();

	void setActiveViewport(const ctl::RcPtr<Direct3D11Viewport> &viewport);

	bool isViewportActive(const ctl::RcPtr<Direct3D11Viewport> &viewport) const;

private:
	ctl::WeakRcPtr<Direct3D11Viewport> _activeViewport;

	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
	Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGIAdapter> _dxgiAdapter;
	Microsoft::WRL::ComPtr<IDCompositionDevice> _compositionDevice;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _vertexInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer;

	InputMesh _previousMesh{};

	void createDevice();

	void createCompositionDevice();

	void setupShaders();

	void setupSamplerState();

	void setupVertexInputLayout();

	void setupVertexBuffer(InputMesh::index_t vertexCount);

	void setupIndexBuffer(InputMesh::index_t indexCount);

	void setupConstantBuffer();

	void updateConstantBuffer(const ConstantBufferData &data);
};

} // namespace cutegfx