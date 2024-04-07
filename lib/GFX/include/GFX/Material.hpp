#pragma once

#include <glm/vec3.hpp>

namespace Gaze::GFX {
	struct Material
	{
		glm::vec3 diffuse = { 1.F, 1.F, 1.F };
		glm::vec3 specular;
		float     shininess;
	};
}
