#include "cutegfx/NinePatchMetrics.hpp"

using namespace cutegfx;

NinePatchMetrics::NinePatchMetrics(const InitData &initData) {
	glm::vec2 scale = 1.f / glm::vec2(initData.atlasSize);

	uv0 = glm::vec2(initData.offset) * scale;
	uv3 = uv0 + glm::vec2(initData.size) * scale;

	uv1 = uv0 + glm::vec2(initData.border.x, initData.border.y) * scale;
	uv2 = uv3 - glm::vec2(initData.border.z, initData.border.w) * scale;

	borderLeftTop = {initData.border.x, initData.border.y};
	borderRightBottom = {initData.border.z, initData.border.w};
}