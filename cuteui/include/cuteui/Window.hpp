#pragma once

#include <memory>
#include <string>

#include <glm/vec2.hpp>
#include <cutegfx/Viewport.hpp>
#include <cutegfx/Window.hpp>
#include <cuteutil/SharedObject.hpp>

#include "Widget.hpp"

class WindowManager;

class Window : public cuteutil::SharedObject<Window>, public Widget {
	friend class WindowManager;

public:
	using BackdropType = cutegfx::Window::BackdropType;
	using DecorationMode = cutegfx::Window::DecorationMode;

	Window(glm::ivec2 size, const std::string &title);

	~Window() override;

	bool isVisible() const override;

	void setVisible(bool visible) override;

	void updateLayout(glm::ivec2 position, glm::ivec2 maxSize) override;

	void updateLayout();

	void draw(cutegfx::Renderer &renderer) override;

	BackdropType getBackdropType() const;

	bool setBackdropType(BackdropType backdropType);

	void setMainWidget(std::shared_ptr<Widget> widget);

private:
	std::shared_ptr<cutegfx::Window> _platformWindow;
	std::shared_ptr<cutegfx::Viewport> _viewport;
	std::shared_ptr<Widget> _mainWidget;
	glm::vec2 _size{};

	void onVisibilityChange(const bool &visible);

	void onFocus();

	void present(bool waitSync = true);
};