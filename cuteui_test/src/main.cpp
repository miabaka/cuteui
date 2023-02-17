#include <cuteui/Application.hpp>
#include <cuteui/Box.hpp>
#include <cuteui/Button.hpp>
#include <cuteui/Spacer.hpp>
#include <cuteui/Window.hpp>

class MyCuteApp : public Application {
public:
	MyCuteApp()  {
		auto window = Window::create(glm::ivec2{640, 400}, "Мия бака!");
		auto secondWindow = Window::create(glm::ivec2{640, 400}, "А может даже ьака");

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

		window->setMainWidget(box);

		window->setVisible(true);
		secondWindow->setVisible(true);
	}
};

int main() {
	return MyCuteApp().run();
}