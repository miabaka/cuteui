#include <cuteui/Application.hpp>
#include <cuteui/Box.hpp>
#include <cuteui/ColoredThingy.hpp>
#include <cuteui/Window.hpp>

class MyCuteApp : public Application {
public:
	MyCuteApp() {
		auto row = std::make_shared<Box>(Direction::Horizontal);

		row->add(std::make_shared<ColoredThingy>(glm::vec4{1.f, 0.f, 0.f, 1.f}));
		row->add(std::make_shared<ColoredThingy>(glm::vec4{0.f, 1.f, 0.f, 1.f}));

		auto column = std::make_shared<Box>(Direction::Vertical);

		column->setHorizontalAlignment(Alignment::Center);
		column->setVerticalAlignment(Alignment::Center);

		column->add(std::make_shared<ColoredThingy>(glm::vec4{0.75f, 0.5f, 1.f, 1.f}));
		column->add(row);
		column->add(std::make_shared<ColoredThingy>(glm::vec4{0.f, 0.f, 1.f, 1.f}));

		auto window = Window::create(glm::ivec2{640, 400}, "Мия пишет гуи");

		window->setMainWidget(column);
		window->setVisible(true);
	}
};

int main() {
	return MyCuteApp().run();
}