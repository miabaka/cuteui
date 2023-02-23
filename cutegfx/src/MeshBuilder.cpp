#include "cutegfx/MeshBuilder.hpp"

using namespace cutegfx;

MeshBuilder::MeshBuilder() {
	_vertices.reserve(2048);
	_indices.reserve(8192);
	_availableVertices = _vertices.capacity();
	_availableIndices = _indices.capacity();
}

MeshBuilder::index_t MeshBuilder::addRect(glm::vec2 p1, glm::vec2 p2, glm::vec4 color) {
	index_t leftTop = emitVertex({p1, color});
	index_t rightTop = emitVertex({{p2.x, p1.y}, color});
	index_t leftBottom = emitVertex({{p1.x, p2.y}, color});
	index_t rightBottom = emitVertex({p2, color});
	return emitIndices(leftTop, rightTop, leftBottom, leftBottom, rightTop, rightBottom);
}

void MeshBuilder::reset() {
	_vertices.clear();
	_indices.clear();
	_availableVertices = _vertices.capacity();
	_availableIndices = _indices.capacity();
	_currentVertexIndex = 0;
	_currentIndex = 0;
}

void MeshBuilder::uploadAndReset(Buffer &vertexBuffer, Buffer &indexBuffer) {
	vertexBuffer.setData(_vertices.data(), sizeof(Vertex) * _vertices.size());
	indexBuffer.setData(_indices.data(), sizeof(index_t) * _indices.size());
	reset();
}

MeshBuilder::index_t MeshBuilder::currentIndex() const {
	return _currentIndex;
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