#include <cute/ui/Application.hpp>
#include <cute/ui/Box.hpp>
#include <cute/ui/Button.hpp>
#include <cute/ui/Spacer.hpp>
#include <cute/ui/Window.hpp>

class MyCuteApp : public Application {
public:
	MyCuteApp() : _window(640, 400, "Мия бака!") {
		auto actuallyWorkingButton = std::make_shared<Button>("Мяу");

		auto box = std::make_shared<Box>(Direction::Vertical);

		box->add(std::make_shared<Spacer>(5));

		box->add(std::make_shared<Button>("Сделать кусь"));
		box->add(std::make_shared<Button>("Погладить"));
		box->add(std::make_shared<Button>("Обнять"));
		box->add(std::make_shared<Button>("Отправить в horny jail"));

		box->add(std::make_shared<Spacer>(1));

		box->add(actuallyWorkingButton);

		box->add(std::make_shared<Spacer>(5));

		_window.setMainWidget(box);

		_window.show();
	}

private:
	Window _window;
};

int main() {
	MyCuteApp().run();
}