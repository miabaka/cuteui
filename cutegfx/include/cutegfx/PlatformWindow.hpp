#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <cuteutil/SingleSignal.hpp>

class PlatformWindow {
public:
	enum class BackdropType {
		Default,
		Mica
	};

	enum class BorderType {
		Default,
		WithoutCaption
	};

	virtual ~PlatformWindow() = default;

	virtual glm::ivec2 getClientSize() const = 0;

	virtual void setClientSize(glm::ivec2 size) = 0;

	virtual void setTitle(const std::string &title) = 0;

	virtual BackdropType getBackdropType() const = 0;

	virtual bool setBackdropType(BackdropType backdropType) = 0;

	virtual bool isVisible() const = 0;

	virtual void setVisible(bool visible) = 0;

	cuteutil::SingleSignal<bool> visibilityChanged;
};