#include "cutegfx/Renderer.hpp"

using namespace cutegfx;

template<class... Ts>
struct overloaded : Ts ... {
	using Ts::operator()...;
};

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Renderer::Renderer(std::shared_ptr<Device> device)
		: _device(std::move(device)) {}

std::shared_ptr<Viewport> Renderer::createViewport() {
	return _device->createViewport();
}

void Renderer::setViewport(std::shared_ptr<Viewport> viewport) {
	_commandList.emplace_back(SetViewportRenderCommand{std::move(viewport)});
}

void Renderer::resize(glm::uvec2 size) {
	_commandList.emplace_back(ResizeRenderCommand{size});
}

void Renderer::clear(glm::vec4 color) {
	_commandList.emplace_back(ClearRenderCommand{color});
}

void Renderer::render() {
	std::shared_ptr<Viewport> viewport;

	for (auto &command: _commandList) {
		std::visit(overloaded{
				[&viewport](SetViewportRenderCommand &setViewport) { viewport = setViewport.viewport; },
				[&viewport](ClearRenderCommand &clear) { viewport->clear(clear.color); },
				[&viewport](ResizeRenderCommand &resize) { viewport->resize(resize.size); }
		}, command);
	}

	_commandList.clear();
}