#pragma once

#include <glm/vec3.hpp>

namespace Gaze::GFX {
	struct Material
	{
		glm::vec3 diffuse;
		glm::vec3 specular;
		float     shininess;
	};
}
