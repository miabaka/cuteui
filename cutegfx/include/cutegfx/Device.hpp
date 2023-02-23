#pragma once

#include <memory>

#include "Buffer.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"

namespace cutegfx {

class Device {
public:
	using index_t = uint32_t;

	virtual ~Device() = default;

	virtual Shader::Format getShaderFormat() const = 0;

	virtual std::shared_ptr<Buffer> createBuffer(Buffer::Type type) = 0;

	virtual std::shared_ptr<Shader> createShader(Shader::Type type, const void *bytecode, size_t size) = 0;

	virtual std::shared_ptr<Viewport> createViewport() = 0;

	virtual void draw(index_t firstIndex, index_t indexCount) = 0;
};

} // namespace cutegfx