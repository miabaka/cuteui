#pragma once

#include <memory>
#include <vector>

#include "DeviceObject.hpp"
#include "Shader.hpp"

namespace cutegfx {

class Buffer : public DeviceObject {
public:
	enum class Type {
		Constant,
		Index,
		Vertex
	};

	enum class ElementType {
		Float2,
		Float4
	};

	struct ElementDesc {
		const char *name;
		ElementType type;
	};

	virtual void setLayout(const std::vector<ElementDesc> &elements, ctl::RcPtr<Shader> vertexShader) = 0;

	virtual void setData(const void *data, size_t size) = 0;

	virtual void use() = 0;
};

} // namespace cutegfx