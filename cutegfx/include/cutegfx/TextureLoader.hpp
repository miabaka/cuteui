#pragma once

#include <filesystem>
#include <ctl/memory.hpp>

#include "Device.hpp"
#include "Texture.hpp"

namespace cutegfx {

class TextureLoader : public ctl::RcObject {
public:
	explicit TextureLoader(const ctl::RcPtr<Device> &device);

	ctl::RcPtr<Texture> loadFromFile(const std::filesystem::path &path);

private:
	ctl::RcPtr<Device> _device;
};

} // namespace cutegfx