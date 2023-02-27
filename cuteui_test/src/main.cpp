#include <cuteui/Application.hpp>
#include <cuteui/Box.hpp>
#include <cuteui/Button.hpp>
#include <cuteui/Window.hpp>

class MyCuteApp : public Application {
public:
	MyCuteApp() : _toggleableWindow(Window::create(glm::ivec2{256, 256}, "Я ьака")) {
		auto button = std::make_shared<Button>();

		button->setHorizontalAlignment(Alignment::Center);
		button->setVerticalAlignment(Alignment::Center);

		button->sClick.bind([&]() {
			_toggleableWindow->setVisible(!_toggleableWindow->isVisible());
		});

		auto window = Window::create(glm::ivec2{640, 400}, "Мия пишет гуи");

		window->setMainWidget(button);
		window->setVisible(true);
	}

private:
	std::shared_ptr<Window> _toggleableWindow;
};

int main() {
	return MyCuteApp().run();
}