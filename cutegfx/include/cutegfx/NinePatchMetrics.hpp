#pragma once

#include <glm/glm.hpp>

namespace cutegfx {

struct NinePatchMetrics {
	struct InitData {
		glm::uvec4 border;
		glm::uvec2 offset;
		glm::uvec2 size;
		glm::uvec2 atlasSize;
	};

	glm::vec2 uv0{};
	glm::vec2 uv1{};
	glm::vec2 uv2{};
	glm::vec2 uv3{};
	glm::vec2 borderLeftTop{};
	glm::vec2 borderRightBottom{};

	NinePatchMetrics() = default;

	explicit NinePatchMetrics(const InitData &initData);
};

} // namespace cutegfx