#pragma once

#include <deque>
#include <variant>
#include <vector>

#include <glm/vec4.hpp>
#include <ctl/memory.hpp>

#include "Device.hpp"
#include "MeshBuilder.hpp"

namespace cutegfx {

class Renderer : public ctl::RcObject {
public:
	explicit Renderer(const ctl::RcPtr<Device> &device);

	ctl::RcPtr<Viewport> createViewport();

	void setViewport(const ctl::RcPtr<Viewport> &viewport);

	void resize(glm::uvec2 size);

	void clear(glm::vec4 color);

	void fillRect(glm::vec2 p1, glm::vec2 p2, glm::u8vec4 color);

	void render();

private:
	struct SetViewportCommand {
		ctl::RcPtr<Viewport> viewport;
	};

	struct ResizeCommand {
		glm::uvec2 size;
	};

	struct ClearCommand {
		glm::vec4 color;
	};

	struct DrawMeshCommand {
		MeshBuilder::index_t firstIndex;
		MeshBuilder::index_t indexCount;
	};

	using RenderCommand = std::variant<SetViewportCommand, ResizeCommand, ClearCommand, DrawMeshCommand>;

	ctl::RcPtr<Device> _device;
	std::deque<RenderCommand> _commandList;
	MeshBuilder _meshBuilder;
	bool _hasIncompleteMesh = false;
	MeshBuilder::index_t _firstIncompleteMeshIndex = 0;

	void submitCurrentMesh();
};

} // namespace cutegfx