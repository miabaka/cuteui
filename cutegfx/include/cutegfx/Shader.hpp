#pragma once

#include "DeviceObject.hpp"

namespace cutegfx {

class Shader : public DeviceObject {
public:
	enum class Format {
		Glsl,
		Hlsl
	};

	enum class Type {
		Pixel,
		Vertex
	};

	virtual Format getFormat() const = 0;

	virtual Type getType() const = 0;

	// TODO: return something like a span
	virtual const void *getBytecode() const = 0;

	virtual size_t getBytecodeSize() const = 0;

	virtual void use() = 0;
};

} // namespace cutegfx