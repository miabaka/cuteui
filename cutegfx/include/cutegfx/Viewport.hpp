#pragma once

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Window.hpp"

namespace cutegfx {

class Viewport {
public:
	virtual ~Viewport() = default;

	virtual void setOutputWindow(std::shared_ptr<Window> window) = 0;

	virtual void clear(glm::vec4 color) = 0;

	virtual void resize(glm::uvec2 size) = 0;

	virtual void present(bool waitSync) = 0;

	virtual void use() = 0;
};

} // namespace cutegfx