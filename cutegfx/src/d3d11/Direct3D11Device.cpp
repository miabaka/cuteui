#include "Direct3D11Device.hpp"

#include <array>
#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "shaders.hpp"
#include "Direct3D11Texture.hpp"

using namespace cutegfx;
using Microsoft::WRL::ComPtr;

Direct3D11Device::Direct3D11Device() {
	createDevice();
	createCompositionDevice();
	setupShaders();
	setupSamplerState();
	setupVertexInputLayout();
	setupConstantBuffer();
}

ctl::RcPtr<Viewport> Direct3D11Device::createViewport() {
	return ctl::rcNew<Direct3D11Viewport>(this);
}

ctl::RcPtr<Texture> Direct3D11Device::createTexture() {
	return ctl::rcNew<Direct3D11Texture>(this);
}

ctl::RcPtr<Texture> Direct3D11Device::createTexture(glm::uvec2 size, const void *data) {
	return ctl::rcNew<Direct3D11Texture>(this, size, data);
}

template<typename T>
static void copyElements(const T *src, void *dst, size_t count) {
	std::memcpy(dst, src, sizeof(T) * count);
}

void Direct3D11Device::setMesh(const InputMesh &mesh) {
	if (!(mesh.vertexCount > 0 && mesh.indexCount > 0))
		return;

	if (mesh.vertexCount > _previousMesh.vertexCount)
		setupVertexBuffer(mesh.vertexCount);

	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		_deviceContext->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

		copyElements(mesh.vertices, mapped.pData, mesh.vertexCount);

		_deviceContext->Unmap(_vertexBuffer.Get(), 0);
	}

	if (mesh.indexCount > _previousMesh.indexCount)
		setupIndexBuffer(mesh.indexCount);

	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		_deviceContext->Map(_indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

		copyElements(mesh.indices, mapped.pData, mesh.indexCount);

		_deviceContext->Unmap(_indexBuffer.Get(), 0);
	}

	_previousMesh = mesh;
}

void Direct3D11Device::draw(InputMesh::index_t firstIndex, InputMesh::index_t indexCount) {
	_deviceContext->DrawIndexed(indexCount, firstIndex, 0);
}

ComPtr<ID3D11Device> Direct3D11Device::getRawDevice() {
	return _device;
}

ComPtr<ID3D11DeviceContext> Direct3D11Device::getRawDeviceContext() {
	return _deviceContext;
}

ComPtr<IDCompositionDevice> Direct3D11Device::getCompositionDevice() {
	return _compositionDevice;
}

ComPtr<IDXGIAdapter> Direct3D11Device::getDxgiAdapter() {
	return _dxgiAdapter;
}

void Direct3D11Device::setActiveViewport(const ctl::RcPtr<Direct3D11Viewport> &viewport) {
	_activeViewport = viewport;
	glm::vec2 viewportSize = viewport->getSize();
	updateConstantBuffer({glm::ortho(0.f, viewportSize.x, viewportSize.y, 0.f), true});
}

bool Direct3D11Device::isViewportActive(const ctl::RcPtr<Direct3D11Viewport> &viewport) const {
	return viewport == _activeViewport.lock();
}

void Direct3D11Device::createDevice() {
	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifndef NDEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result = D3D11CreateDevice(
			nullptr, // adapter
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, // software
			flags,
			nullptr, 0, // featureLevels
			D3D11_SDK_VERSION,
			&_device,
			nullptr, // featureLevel
			&_deviceContext
	);

	if (!SUCCEEDED(result))
		throw std::runtime_error("D3D11CreateDevice failed");

	_device->QueryInterface(__uuidof(IDXGIDevice), &_dxgiDevice);
	_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &_dxgiAdapter);
}

void Direct3D11Device::createCompositionDevice() {
	HRESULT result = DCompositionCreateDevice(_dxgiDevice.Get(), __uuidof(IDCompositionDevice), &_compositionDevice);

	if (!SUCCEEDED(result))
		throw std::runtime_error("DCompositionCreateDevice failed");
}

void Direct3D11Device::setupShaders() {
	HRESULT result;

	result = _device->CreateVertexShader(
			VERTEX_SHADER_CODE,
			sizeof(VERTEX_SHADER_CODE),
			nullptr, // classLinkage
			&_vertexShader
	);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreateVertexShader failed");

	result = _device->CreatePixelShader(
			PIXEL_SHADER_CODE,
			sizeof(PIXEL_SHADER_CODE),
			nullptr, // classLinkage
			&_pixelShader
	);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreatePixelShader failed");

	_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
	_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
}

void Direct3D11Device::setupSamplerState() {
	D3D11_SAMPLER_DESC samplerDesc{};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	HRESULT result = _device->CreateSamplerState(&samplerDesc, &_samplerState);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreateSamplerState failed");

	_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
}

void Direct3D11Device::setupVertexInputLayout() {
	using ElementDesc = D3D11_INPUT_ELEMENT_DESC;

	const auto APPEND = D3D11_APPEND_ALIGNED_ELEMENT;
	const auto PER_VERTEX = D3D11_INPUT_PER_VERTEX_DATA;

	std::array elementDescs = {
			ElementDesc{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, APPEND, PER_VERTEX, 0},
			ElementDesc{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, APPEND, PER_VERTEX, 0},
			ElementDesc{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, APPEND, PER_VERTEX, 0}
	};

	HRESULT result = _device->CreateInputLayout(
			elementDescs.data(),
			elementDescs.size(),
			VERTEX_SHADER_CODE,
			sizeof(VERTEX_SHADER_CODE),
			&_vertexInputLayout
	);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreateInputLayout failed");
}

void Direct3D11Device::setupVertexBuffer(InputMesh::index_t vertexCount) {
	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = sizeof(InputMesh::Vertex) * vertexCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = _device->CreateBuffer(&desc, nullptr, &_vertexBuffer);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreateBuffer failed");

	UINT stride = sizeof(InputMesh::Vertex);
	UINT offset = 0;

	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetInputLayout(_vertexInputLayout.Get());
}

void Direct3D11Device::setupIndexBuffer(InputMesh::index_t indexCount) {
	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = sizeof(InputMesh::index_t) * indexCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = _device->CreateBuffer(&desc, nullptr, &_indexBuffer);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreateBuffer failed");

	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Direct3D11Device::setupConstantBuffer() {
	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = sizeof(ConstantBufferData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT result = _device->CreateBuffer(&desc, nullptr, &_constantBuffer);

	if (FAILED(result))
		throw std::runtime_error("ID3D11Device::CreateBuffer failed");

	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	_deviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
}

void Direct3D11Device::updateConstantBuffer(const ConstantBufferData &data) {
	D3D11_MAPPED_SUBRESOURCE mapped;
	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	copyElements(&data, mapped.pData, 1);

	_deviceContext->Unmap(_constantBuffer.Get(), 0);
}