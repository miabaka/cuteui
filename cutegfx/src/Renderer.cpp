#include "cutegfx/Renderer.hpp"

using namespace cutegfx;

template<class... Ts>
struct overloaded : Ts ... {
	using Ts::operator()...;
};

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Renderer::Renderer(const std::shared_ptr<Device> &device)
		: _device(device),
		  _vertexBuffer(device->createBuffer(Buffer::Type::Vertex)),
		  _indexBuffer(device->createBuffer(Buffer::Type::Index)) {}

std::shared_ptr<Viewport> Renderer::createViewport() {
	return _device->createViewport();
}

void Renderer::setViewport(std::shared_ptr<Viewport> viewport) {
	submitCurrentMesh();
	_commandList.emplace_back(SetViewportCommand{std::move(viewport)});
}

void Renderer::resize(glm::uvec2 size) {
	submitCurrentMesh();
	_commandList.emplace_back(ResizeCommand{size});
}

void Renderer::clear(glm::vec4 color) {
	submitCurrentMesh();
	_commandList.emplace_back(ClearCommand{color});
}

void Renderer::fillRect(glm::vec2 p1, glm::vec2 p2, glm::vec4 color) {
	MeshBuilder::index_t firstIndex = _meshBuilder.addRect(p1, p2, color);

	if (!_hasIncompleteMesh)
		_firstIncompleteMeshIndex = firstIndex;

	_hasIncompleteMesh = true;
}

void Renderer::render() {
	submitCurrentMesh();
	_meshBuilder.uploadAndReset(*_vertexBuffer, *_indexBuffer);

	std::shared_ptr<Viewport> viewport;

	for (auto &command: _commandList) {
		std::visit(overloaded{
				[&](SetViewportCommand &cmd) { viewport = cmd.viewport; },
				[&](ClearCommand &cmd) { viewport->clear(cmd.color); },
				[&](ResizeCommand &cmd) { viewport->resize(cmd.size); },
				[&](DrawMeshCommand &cmd) {}
		}, command);
	}

	_commandList.clear();
}

void Renderer::submitCurrentMesh() {
	if (!_hasIncompleteMesh)
		return;

	_hasIncompleteMesh = false;

	DrawMeshCommand command{};

	command.firstIndex = _firstIncompleteMeshIndex;
	command.indexCount = _meshBuilder.currentIndex() - command.firstIndex;

	_commandList.emplace_back(command);
}