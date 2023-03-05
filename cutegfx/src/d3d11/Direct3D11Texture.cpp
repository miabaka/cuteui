#include "Direct3D11Texture.hpp"

#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Direct3D11Device.hpp"

using namespace cutegfx;

Direct3D11Texture::Direct3D11Texture(const ctl::RcPtr<Direct3D11Device> &device)
		: _device(device),
		  _rawDevice(device->getRawDevice()),
		  _rawContext(device->getRawDeviceContext()) {
	createTexture();
	createResourceView();
}

void Direct3D11Texture::use() {
	_rawContext->PSSetShaderResources(0, 1, _rawResourceView.GetAddressOf());
}

void Direct3D11Texture::createTexture() {
	D3D11_TEXTURE2D_DESC textureDesc{};

	textureDesc.Width = 1;
	textureDesc.Height = 1;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	glm::u8vec4 color = {0xff, 0x00, 0xff, 0xff};
	D3D11_SUBRESOURCE_DATA initialData = {glm::value_ptr(color), sizeof(color)};

	HRESULT result = _rawDevice->CreateTexture2D(&textureDesc, &initialData, &_rawTexture);

	if (!SUCCEEDED(result))
		throw std::runtime_error("ID3D11Device::CreateTexture2D failed");
}

void Direct3D11Texture::createResourceView() {
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};

	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	HRESULT result = _rawDevice->CreateShaderResourceView(_rawTexture.Get(), &viewDesc, &_rawResourceView);

	if (!SUCCEEDED(result))
		throw std::runtime_error("ID3D11Device::CreateShaderResourceView failed");
}