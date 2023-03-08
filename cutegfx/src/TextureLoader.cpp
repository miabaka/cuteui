#include "cutegfx/TextureLoader.hpp"

#include <fstream>
#include <stb/image.hpp>

namespace fs = std::filesystem;
namespace stbi = stb::image;

using namespace cutegfx;

TextureLoader::TextureLoader(const ctl::RcPtr<Device> &device)
		: _device(device) {}

ctl::RcPtr<Texture> TextureLoader::loadFromFile(const fs::path &path) {
	std::ifstream file(path, std::ios::binary);

	stbi::Result result = stbi::loadFromStream(file, stbi::ColorFormat::Rgba);

	if (!result.data)
		return _device->createTexture();

	return _device->createTexture(result.size, result.data);
}