#include <cuteui/Application.hpp>
#include <cuteui/Box.hpp>
#include <cuteui/Button.hpp>
#include <cuteui/Window.hpp>

class MyCuteApp : public Application {
public:
	MyCuteApp() {
		auto box = ctl::RcPtr<Box>::alloc(Direction::Vertical, 6);

		box->setHorizontalAlignment(Alignment::Center);
		box->setVerticalAlignment(Alignment::Center);

		for (int i = 0; i < 10; i++)
			box->add(ctl::RcPtr<Button>::alloc());

		auto window = ctl::RcPtr<Window>::alloc(glm::ivec2{640, 400}, "Мия пишет гуи");

		window->setMainWidget(box);
		window->setVisible(true);
	}
};

int main() {
	return MyCuteApp().run();
}