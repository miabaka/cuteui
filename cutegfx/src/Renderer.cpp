#include "cutegfx/Renderer.hpp"

using namespace cutegfx;

Renderer::Renderer(std::shared_ptr<Device> device)
		: _device(std::move(device)) {}

std::shared_ptr<Viewport> Renderer::createViewport() {
	return _device->createViewport();
}

void Renderer::render() {
}