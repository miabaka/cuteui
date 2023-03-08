#pragma once

#include <cstdint>
#include <glm/glm.hpp>

struct InputMesh {
	using index_t = uint32_t;

#pragma pack(push, 1)
	struct Vertex {
		glm::vec2 position;
		glm::vec2 uv;
		glm::u8vec4 color;
	};
#pragma pack(pop)

	const Vertex *vertices;
	const index_t *indices;
	index_t vertexCount;
	index_t indexCount;
};