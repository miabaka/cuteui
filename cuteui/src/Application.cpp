#include "cuteui/Application.hpp"

#include <stdexcept>

Application *Application::_instance = nullptr;

Application &Application::getInstance() {
	if (!_instance)
		throw std::runtime_error("You must first create an Application");

	return *_instance;
}

Application::Application() {
	if (_instance)
		throw std::runtime_error("Multiple Application instances can not exist simultaneously");

	_platform = cutegfx::Platform::createBest();

	if (!_platform)
		throw std::runtime_error("Failed to create cutegfx platform");

	prepareTheme();

	_instance = this;
}

Application::~Application() {
	_instance = nullptr;
}

int Application::run() {
	_windowManager.sUpdate.bind([this](WindowManager::UpdateType type) {
		_platform->executeTickHandlerIndirect(type);
	});

	_windowManager.startRenderThread();

	int result = _platform->runEventLoop([this]() {
		_windowManager.updateWindows();
	});

	_windowManager.joinRenderThread();

	return result;
}

cutegfx::Platform &Application::getPlatform() {
	return *_platform;
}

WindowManager &Application::getWindowManager() {
	return _windowManager;
}

Theme &Application::getTheme() {
	return _theme;
}

void Application::prepareTheme() {
	cutegfx::TextureLoader &textureLoader = _platform->getTextureLoader();

	Theme::NinePatchImage image{};

	image.texture = textureLoader.loadFromFile("data/test.png");

	cutegfx::NinePatchMetrics::InitData metricsInit{};

	metricsInit.border = {4, 4, 4, 4};
	metricsInit.offset = {0, 0};
	metricsInit.size = {16, 16};
	metricsInit.atlasSize = {16, 16};

	image.metrics = cutegfx::NinePatchMetrics(metricsInit);

	_theme.images.buttonNormal = image;
}