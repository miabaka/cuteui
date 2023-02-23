#pragma once

class Buffer {
public:
	enum class Type {
		Index,
		Vertex
	};

	virtual ~Buffer() = default;

	virtual void setData(const void *data, size_t size) = 0;
};