#pragma once

#include <fstream>

#include <glm/glm.hpp>

namespace stb::image {

enum class ColorFormat {
	Any = 0,
	Grey = 1,
	GreyAlpha = 2,
	Rgb = 3,
	Rgba = 4
};

struct Result {
	void *data = nullptr;
	glm::ivec2 size = {};
	ColorFormat colorFormat = ColorFormat::Any;

	Result() = default;

	Result(Result &) = delete;

	Result(Result &&) noexcept;

	~Result();
};

Result getInfoFromStream(std::ifstream &stream);

Result loadFromStream(std::ifstream &stream, ColorFormat requiredColorFormat = ColorFormat::Any);

} // namespace stb::image