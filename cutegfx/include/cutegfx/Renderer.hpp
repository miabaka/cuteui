#pragma once

#include <deque>
#include <memory>
#include <variant>

#include <glm/vec4.hpp>

#include "Device.hpp"

namespace cutegfx {

class Renderer {
public:
	explicit Renderer(std::shared_ptr<Device> device);

	std::shared_ptr<Viewport> createViewport();

	void setViewport(std::shared_ptr<Viewport> viewport);

	void resize(glm::uvec2 size);

	void clear(glm::vec4 color);

	void render();

private:
	struct SetViewportRenderCommand {
		std::shared_ptr<Viewport> viewport;
	};

	struct ResizeRenderCommand {
		glm::uvec2 size;
	};

	struct ClearRenderCommand {
		glm::vec4 color;
	};

	using RenderCommand = std::variant<
			SetViewportRenderCommand,
			ResizeRenderCommand,
			ClearRenderCommand
	>;

	std::shared_ptr<Device> _device;
	std::deque<RenderCommand> _commandList;
};

} // namespace cutegfx