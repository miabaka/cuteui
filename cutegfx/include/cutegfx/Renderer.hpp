#pragma once

#include <deque>
#include <variant>
#include <vector>

#include <glm/vec4.hpp>
#include <ctl/memory.hpp>

#include "Device.hpp"
#include "MeshBuilder.hpp"
#include "NinePatchMetrics.hpp"

namespace cutegfx {

class Renderer : public ctl::RcObject {
public:
	explicit Renderer(const ctl::RcPtr<Device> &device);

	ctl::RcPtr<Texture> createTexture();

	ctl::RcPtr<Viewport> createViewport();

	void setTexture(const ctl::RcPtr<Texture> &texture);

	void setViewport(const ctl::RcPtr<Viewport> &viewport);

	void resize(glm::uvec2 size);

	void clear(glm::vec4 color);

	void fillRect(glm::vec2 p1, glm::vec2 p2, glm::u8vec4 color);

	void drawNinePatch(glm::vec2 p1, glm::vec2 p2, const NinePatchMetrics &metrics);

	void beginFrame();

	void render();

private:
	struct SetTextureCommand {
		ctl::RcPtr<Texture> texture;
	};

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

	using RenderCommand = std::variant<
			SetViewportCommand,
			ResizeCommand,
			ClearCommand,
			DrawMeshCommand,
			SetTextureCommand
	>;

	ctl::RcPtr<Device> _device;
	ctl::RcPtr<Texture> _lastTexture;
	ctl::RcPtr<Texture> _sampleTexture;
	std::deque<RenderCommand> _commandList;
	MeshBuilder _meshBuilder;
	bool _hasIncompleteMesh = false;
	MeshBuilder::index_t _firstIncompleteMeshIndex = 0;

	void submitCurrentMesh();
};

} // namespace cutegfx