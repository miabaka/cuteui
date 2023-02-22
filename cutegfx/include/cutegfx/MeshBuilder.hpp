#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

class MeshBuilder {
public:
#pragma pack(push, 1)
	struct Vertex {
		glm::vec2 position;
		glm::vec4 color;
	};
#pragma pack(pop)

	using index_t = uint32_t;

	MeshBuilder();

	index_t addRect(glm::vec2 p1, glm::vec2 p2, glm::vec4 color);

	void reset();

	index_t currentIndex() const;

private:
	std::vector<Vertex> _vertices;
	std::vector<index_t> _indices;
	size_t _availableVertices{};
	index_t _availableIndices{};
	index_t _currentIndex = 0;

	index_t emitVertex(const Vertex &vertex);

	template<typename ...TIndices>
	void emitIndices(const TIndices &...indices) {
		while (_availableIndices < sizeof...(indices)) {
			_indices.reserve(_indices.capacity() * 2);
			_availableIndices = _indices.capacity() - _indices.size();
		}

		for (auto index: {indices...})
			_indices.push_back(index);

		_availableIndices -= sizeof...(indices);
	}
};