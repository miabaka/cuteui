#pragma once

#include "cutegfx/NinePatchMetrics.hpp"
#include "cutegfx/Texture.hpp"

struct Theme {
public:
	struct NinePatchImage {
		cutegfx::NinePatchMetrics metrics;
		ctl::RcPtr<cutegfx::Texture> texture;
	};

	struct {
		NinePatchImage buttonNormal;
	} images;
};