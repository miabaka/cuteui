#include "cutegfx/MeshBuilder.hpp"

using namespace cutegfx;

MeshBuilder::MeshBuilder() {
	_vertices.reserve(2048);
	_indices.reserve(8192);
	_availableVertices = _vertices.capacity();
	_availableIndices = _indices.capacity();
}

MeshBuilder::index_t MeshBuilder::addRect(glm::vec2 p1, glm::vec2 p2, glm::u8vec4 color) {
	index_t leftTop = emitVertex({p1, {0.f, 0.f}, color});
	index_t rightTop = emitVertex({{p2.x, p1.y}, {1.f, 0.f}, color});
	index_t leftBottom = emitVertex({{p1.x, p2.y}, {0.f, 1.f}, color});
	index_t rightBottom = emitVertex({p2, {1.f, 1.f}, color});
	return emitIndices(leftTop, rightTop, leftBottom, leftBottom, rightTop, rightBottom);
}

MeshBuilder::index_t MeshBuilder::addNinePatch(glm::vec2 p0, glm::vec2 p3, const NinePatchMetrics &m) {
	glm::vec2 p1 = p0 + m.borderLeftTop;
	glm::vec2 p2 = p3 - m.borderRightBottom;

	glm::u8vec4 color = {0xff, 0xff, 0xff, 0xff};

	index_t index00 = emitVertex({{p0.x, p0.y}, {m.uv0.x, m.uv0.y}, color});
	index_t index10 = emitVertex({{p1.x, p0.y}, {m.uv1.x, m.uv0.y}, color});
	index_t index20 = emitVertex({{p2.x, p0.y}, {m.uv2.x, m.uv0.y}, color});
	index_t index30 = emitVertex({{p3.x, p0.y}, {m.uv3.x, m.uv0.y}, color});

	index_t index01 = emitVertex({{p0.x, p1.y}, {m.uv0.x, m.uv1.y}, color});
	index_t index11 = emitVertex({{p1.x, p1.y}, {m.uv1.x, m.uv1.y}, color});
	index_t index21 = emitVertex({{p2.x, p1.y}, {m.uv2.x, m.uv1.y}, color});
	index_t index31 = emitVertex({{p3.x, p1.y}, {m.uv3.x, m.uv1.y}, color});

	index_t index02 = emitVertex({{p0.x, p2.y}, {m.uv0.x, m.uv2.y}, color});
	index_t index12 = emitVertex({{p1.x, p2.y}, {m.uv1.x, m.uv2.y}, color});
	index_t index22 = emitVertex({{p2.x, p2.y}, {m.uv2.x, m.uv2.y}, color});
	index_t index32 = emitVertex({{p3.x, p2.y}, {m.uv3.x, m.uv2.y}, color});

	index_t index03 = emitVertex({{p0.x, p3.y}, {m.uv0.x, m.uv3.y}, color});
	index_t index13 = emitVertex({{p1.x, p3.y}, {m.uv1.x, m.uv3.y}, color});
	index_t index23 = emitVertex({{p2.x, p3.y}, {m.uv2.x, m.uv3.y}, color});
	index_t index33 = emitVertex({{p3.x, p3.y}, {m.uv3.x, m.uv3.y}, color});

	return emitIndices(
			// left-top quad
			index00, index10, index11, index11, index01, index00,
			// middle-top quad
			index10, index20, index21, index21, index11, index10,
			// right-top quad
			index20, index30, index31, index31, index21, index20,

			// left-middle quad
			index01, index11, index12, index12, index02, index01,
			// middle-middle quad
			index11, index21, index22, index22, index12, index11,
			// right-middle quad
			index21, index31, index32, index32, index22, index21,

			// left-bottom quad
			index02, index12, index13, index13, index03, index02,
			// middle-bottom quad
			index12, index22, index23, index23, index13, index12,
			// right-bottom quad
			index22, index32, index33, index33, index23, index22
	);
}

void MeshBuilder::reset() {
	_vertices.clear();
	_indices.clear();
	_availableVertices = _vertices.capacity();
	_availableIndices = _indices.capacity();
	_currentVertexIndex = 0;
	_currentIndex = 0;
}

MeshBuilder::index_t MeshBuilder::currentIndex() const {
	return _currentIndex;
}

InputMesh MeshBuilder::getMesh() const {
	InputMesh mesh{};

	mesh.vertices = _vertices.data();
	mesh.vertexCount = _vertices.size();
	mesh.indices = _indices.data();
	mesh.indexCount = _indices.size();

	return mesh;
}

MeshBuilder::index_t MeshBuilder::emitVertex(const Vertex &vertex) {
	if (_availableVertices < 1) {
		_vertices.reserve(_vertices.capacity() * 2);
		_availableVertices = _vertices.capacity() - _vertices.size();
	}

	_vertices.emplace_back(vertex);
	_availableVertices--;

	return _currentVertexIndex++;
}