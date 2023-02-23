#pragma once

#include <deque>
#include <memory>
#include <variant>
#include <vector>

#include <glm/vec4.hpp>

#include "Device.hpp"
#include "MeshBuilder.hpp"
#include "Viewport.hpp"

namespace cutegfx {

class Renderer {
public:
	explicit Renderer(const std::shared_ptr<Device>& device);

	std::shared_ptr<Viewport> createViewport();

	void setViewport(std::shared_ptr<Viewport> viewport);

	void resize(glm::uvec2 size);

	void clear(glm::vec4 color);

	void fillRect(glm::vec2 p1, glm::vec2 p2, glm::vec4 color);

	void render();

private:
	struct SetViewportCommand {
		std::shared_ptr<Viewport> viewport;
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

	std::shared_ptr<Device> _device;
	std::shared_ptr<Buffer> _vertexBuffer;
	std::shared_ptr<Buffer> _indexBuffer;
	std::deque<RenderCommand> _commandList;
	MeshBuilder _meshBuilder;
	bool _hasIncompleteMesh = false;
	MeshBuilder::index_t _firstIncompleteMeshIndex = 0;

	void submitCurrentMesh();
};

} // namespace cutegfx