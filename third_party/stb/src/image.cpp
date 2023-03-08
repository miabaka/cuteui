#include "stb/image.hpp"

#define STBI_NO_THREAD_LOCALS
#define STBI_NO_STDIO

#define STBI_ONLY_PNG

#define STB_IMAGE_IMPLEMENTATION

#include "image.h"

using namespace stb::image;

Result::Result(Result &&other) noexcept
		: data(other.data),
		  size(other.size),
		  colorFormat(other.colorFormat) {
	other.data = nullptr;
	other.size = {};
	other.colorFormat = ColorFormat::Any;
}

Result::~Result() {
	stbi_image_free(data);
}

static int stbi_fstream_read(void *userPtr, char *buf, int count) {
	auto &stream = *static_cast<std::ifstream *>(userPtr);
	return (int) int(stream.read(buf, count).gcount());
}

static void stbi_fstream_skip(void *userPtr, int count) {
	auto &stream = *static_cast<std::ifstream *>(userPtr);

	stream.seekg(count, std::ios_base::cur);

	// have to read a byte to reset eofbit
	if (stream.get() == EOF)
		return;

	// push byte back onto stream if valid.
	stream.unget();
}

static int stbi_fstream_eof(void *userPtr) {
	return static_cast<std::ifstream *>(userPtr)->good();
}

static const stbi_io_callbacks STBI_FSTREAM_CALLBACKS = {
		stbi_fstream_read,
		stbi_fstream_skip,
		stbi_fstream_eof
};

Result stb::image::getInfoFromStream(std::ifstream &stream) {
	Result result{};

	if (!stream)
		return result;

	int channelCount = 0;
	stbi_info_from_callbacks(&STBI_FSTREAM_CALLBACKS, &stream, &result.size.x, &result.size.y, &channelCount);

	result.colorFormat = static_cast<ColorFormat>(channelCount);

	return result;
}

Result stb::image::loadFromStream(std::ifstream &stream, ColorFormat requiredColorFormat) {
	Result result{};

	if (!stream)
		return result;

	int requiredChannelCount = static_cast<int>(requiredColorFormat);
	int channelCount = 0;

	result.data = stbi_load_from_callbacks(
			&STBI_FSTREAM_CALLBACKS,
			&stream, // user
			&result.size.x, &result.size.y,
			&channelCount,
			requiredChannelCount
	);

	result.colorFormat = static_cast<ColorFormat>(channelCount);

	return result;
}