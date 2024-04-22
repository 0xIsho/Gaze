#pragma once

#include <glm/vec3.hpp>

namespace Gaze::GFX {
	/**
	 * @brief A struct representing a light in the scene.
	 */
	struct Light
	{
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 diffuse;
		float     ambientCoefficient;
		float     attenuation;
	};
}
