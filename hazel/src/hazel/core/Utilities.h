#pragma once

#include <glm/glm.hpp>

namespace Hazel::Utilities {

	class Color {
	public:
		static glm::vec4 RGBToVec4RGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
		static glm::vec4 HSVtoVec4RGB(const glm::vec3& hsv);

	private:
	};

}