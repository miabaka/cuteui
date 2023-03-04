#pragma once

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <ctl/memory.hpp>

#include "Window.hpp"

namespace cutegfx {

class Viewport : public ctl::RcObject {
public:
	virtual void setOutputWindow(const ctl::RcPtr<Window> &window) = 0;

	virtual glm::uvec2 getSize() const = 0;

	virtual void setSize(glm::uvec2 size) = 0;

	virtual void clear(glm::u8vec4 color) = 0;

	virtual void present(bool waitSync) = 0;

	virtual void use() = 0;
};

} // namespace cutegfx