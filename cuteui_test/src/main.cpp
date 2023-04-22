#include <cuteui/Application.hpp>
#include <cuteui/Box.hpp>
#include <cuteui/Button.hpp>
#include <cuteui/Window.hpp>

class MyCuteApp : public Application {
public:
	MyCuteApp() {
		auto box = ctl::rcNew<Box>(Direction::Vertical, 6);

		box->setHorizontalAlignment(Alignment::Center);
		box->setVerticalAlignment(Alignment::Center);

		for (int i = 0; i < 10; i++) {
			auto button = ctl::rcNew<Button>();
			button->setHorizontalAlignment(i % 2 == 0 ? Alignment::Start : Alignment::End);
			box->add(button);
		}

		auto horizontalBox = ctl::rcNew<Box>(Direction::Horizontal, 6);

		for (int i = 0; i < 5; i++)
			horizontalBox->add(ctl::rcNew<Button>());

		box->add(horizontalBox);

		auto window = ctl::rcNew<Window>(box->computeRequiredSize() + 18, "Мия пишет гуи");

		window->setMainWidget(box);
		window->setVisible(true);
	}
};

int main() {
	return MyCuteApp().run();
}