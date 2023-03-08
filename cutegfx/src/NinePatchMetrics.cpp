#include "cutegfx/NinePatchMetrics.hpp"

using namespace cutegfx;

NinePatchMetrics::NinePatchMetrics(const InitData &initData) {
	glm::ivec2 integerPoint0 = initData.offset;
	glm::ivec2 integerPoint3 = initData.offset + initData.size;

	glm::ivec2 integerPoint1 = integerPoint0 + glm::ivec2{initData.border.x, initData.border.y};
	glm::ivec2 integerPoint2 = integerPoint3 - glm::ivec2{initData.border.z, initData.border.w};

	borderLeftTop = {initData.border.x, initData.border.y};
	borderRightBottom = {initData.border.z, initData.border.w};

	glm::vec2 scale = 1.f / glm::vec2(initData.atlasSize);

	uv0 = glm::vec2(integerPoint0) * scale;
	uv1 = glm::vec2(integerPoint1) * scale;
	uv2 = glm::vec2(integerPoint2) * scale;
	uv3 = glm::vec2(integerPoint3) * scale;
}