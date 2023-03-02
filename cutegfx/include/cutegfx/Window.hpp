#pragma once

#include <string>
#include <ctl/memory.hpp>

#include <glm/vec2.hpp>
#include <ctl/Signal.hpp>

namespace cutegfx {

class Window : public ctl::RcObject {
public:
	enum class BackdropType {
		Default,
		Mica,
		Transparent,
		TransparentBlur
	};

	enum class DecorationMode {
		Default,
		WithoutCaption,
		Borderless
	};

	virtual glm::ivec2 getClientSize() const = 0;

	virtual void setClientSize(glm::ivec2 size) = 0;

	virtual void setTitle(const std::string &title) = 0;

	virtual BackdropType getBackdropType() const = 0;

	virtual bool setBackdropType(BackdropType backdropType) = 0;

	virtual bool isVisible() const = 0;

	virtual void setVisible(bool visible) = 0;

	ctl::Signal<bool> sVisibilityChange;
	ctl::Signal<> sFocus;
	ctl::Signal<> sResizeBegin;
	ctl::Signal<> sResizeEnd;
	ctl::Signal<glm::ivec2> sMousePress;
	ctl::Signal<glm::ivec2> sMouseRelease;
};

} // namespace cutegfx