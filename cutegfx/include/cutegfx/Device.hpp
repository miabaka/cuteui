#pragma once

#include <memory>
#include <ctl/memory.hpp>

#include "InputMesh.hpp"
#include "Viewport.hpp"

namespace cutegfx {

class Device : public ctl::RcObject {
public:
	virtual ctl::RcPtr<Viewport> createViewport() = 0;

	virtual void setMesh(const InputMesh &mesh) = 0;

	virtual void draw(InputMesh::index_t firstIndex, InputMesh::index_t indexCount) = 0;
};

} // namespace cutegfx