#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include "Device.hpp"
#include "MeshBuilder.hpp"
#include "NinePatchMetrics.hpp"

namespace cutegfx {

class MeshBuilder {
public:
	using index_t = InputMesh::index_t;
	using Vertex = InputMesh::Vertex;

	MeshBuilder();

	index_t addRect(glm::vec2 p1, glm::vec2 p2, glm::u8vec4 color);

	index_t addNinePatch(glm::vec2 p1, glm::vec2 p2, const NinePatchMetrics &metrics);

	void reset();

	index_t currentIndex() const;

	InputMesh getMesh() const;

private:
	std::vector<Vertex> _vertices;
	std::vector<index_t> _indices;
	size_t _availableVertices{};
	index_t _availableIndices{};
	index_t _currentVertexIndex = 0;
	index_t _currentIndex = 0;

	index_t emitVertex(const Vertex &vertex);

	template<typename ...TIndices>
	index_t emitIndices(const TIndices &...indices) {
		index_t firstIndex = _currentIndex;

		while (_availableIndices < sizeof...(indices)) {
			_indices.reserve(_indices.capacity() * 2);
			_availableIndices = _indices.capacity() - _indices.size();
		}

		for (auto index: {indices...})
			_indices.push_back(index);

		_availableIndices -= sizeof...(indices);
		_currentIndex += sizeof...(indices);

		return firstIndex;
	}
};

} // namespace cutegfx