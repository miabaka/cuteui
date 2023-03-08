#include "cutegfx/Renderer.hpp"

using namespace cutegfx;

template<class... Ts>
struct overloaded : Ts ... {
	using Ts::operator()...;
};

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Renderer::Renderer(const ctl::RcPtr<Device> &device)
		: _device(device),
		  _sampleTexture(device->createTexture()) {}

ctl::RcPtr<Device> Renderer::getDevice() const {
	return _device;
}

ctl::RcPtr<Texture> Renderer::createTexture() {
	return _device->createTexture();
}

ctl::RcPtr<Viewport> Renderer::createViewport() {
	return _device->createViewport();
}

void Renderer::setTexture(const ctl::RcPtr<Texture> &texture) {
	if (texture == _lastTexture)
		return;

	submitCurrentMesh();

	_commandList.emplace_back(SetTextureCommand{texture});
	_lastTexture = texture;
}

void Renderer::setViewport(const ctl::RcPtr<Viewport> &viewport) {
	submitCurrentMesh();
	_commandList.emplace_back(SetViewportCommand{viewport});
}

void Renderer::resize(glm::uvec2 size) {
	submitCurrentMesh();
	_commandList.emplace_back(ResizeCommand{size});
}

void Renderer::clear(glm::vec4 color) {
	submitCurrentMesh();
	_commandList.emplace_back(ClearCommand{color});
}

void Renderer::fillRect(glm::vec2 p1, glm::vec2 p2, glm::u8vec4 color) {
	MeshBuilder::index_t firstIndex = _meshBuilder.addRect(p1, p2, color);

	if (!_hasIncompleteMesh)
		_firstIncompleteMeshIndex = firstIndex;

	_hasIncompleteMesh = true;
}

void Renderer::drawNinePatch(glm::vec2 p1, glm::vec2 p2, const NinePatchMetrics &metrics) {
	MeshBuilder::index_t firstIndex = _meshBuilder.addNinePatch(p1, p2, metrics);

	if (!_hasIncompleteMesh)
		_firstIncompleteMeshIndex = firstIndex;

	_hasIncompleteMesh = true;
}

void Renderer::beginFrame() {
	setTexture(_sampleTexture);
}

void Renderer::render() {
	_lastTexture = nullptr;

	submitCurrentMesh();

	_device->setMesh(_meshBuilder.getMesh());
	_meshBuilder.reset();

	ctl::RcPtr<Viewport> viewport;

	for (auto &command: _commandList) {
		std::visit(overloaded{
				[&](SetTextureCommand &cmd) { cmd.texture->use(); },
				[&](SetViewportCommand &cmd) {
					if (cmd.viewport == viewport)
						return;

					viewport = cmd.viewport;
					viewport->use();
				},
				[&](ClearCommand &cmd) {
					if (viewport)
						viewport->clear(cmd.color);
				},
				[&](ResizeCommand &cmd) {
					if (viewport)
						viewport->setSize(cmd.size);
				},
				[&](DrawMeshCommand &cmd) { _device->draw(cmd.firstIndex, cmd.indexCount); }
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