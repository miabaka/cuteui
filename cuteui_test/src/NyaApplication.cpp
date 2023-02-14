#include "NyaApplication.hpp"

using namespace cute::ui;

NyaApplication::NyaApplication()
		: _window(640, 400, "Мия бака!") {
	_window.show();
}