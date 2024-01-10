#pragma once

#include <glm/vec3.hpp>

namespace Gaze::GFX {
	struct Light
	{
		glm::vec3 position;
		glm::vec3 diffuse;
		float     ambientCoefficient;
		float     attenuation;
	};
}
