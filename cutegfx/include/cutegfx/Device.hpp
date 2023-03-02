#pragma once

#include <memory>
#include <ctl/memory.hpp>

#include "Buffer.hpp"
#include "Shader.hpp"
#include "Viewport.hpp"

namespace cutegfx {

class Device : public ctl::RcObject {
public:
	using index_t = uint32_t;

	virtual Shader::Format getShaderFormat() const = 0;

	virtual ctl::RcPtr<Buffer> createBuffer(Buffer::Type type) = 0;

	virtual ctl::RcPtr<Shader> createShader(Shader::Type type, const void *bytecode, size_t size) = 0;

	virtual ctl::RcPtr<Viewport> createViewport() = 0;

	virtual void draw(index_t firstIndex, index_t indexCount) = 0;
};

} // namespace cutegfx