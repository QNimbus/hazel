#include "hzpch.h"
#include "hazel/core/Utilities.h"

namespace Hazel::Utilities {

	glm::vec4 Color::RGBToVec4RGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		float R = red / 256;
		float G = green / 256;
		float B = blue / 256;
		float A = alpha / 256;

		return {R, G, B, A };
	}

	glm::vec4 Color::HSVtoVec4RGB(const glm::vec3& hsv) {
		int H = (int)(hsv.x * 360.0f);
		double S = hsv.y;
		double V = hsv.z;

		double C = S * V;
		double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
		double m = V - C;
		double Rs, Gs, Bs;

		if (H >= 0 && H < 60) {
			Rs = C;
			Gs = X;
			Bs = 0;
		}
		else if (H >= 60 && H < 120) {
			Rs = X;
			Gs = C;
			Bs = 0;
		}
		else if (H >= 120 && H < 180) {
			Rs = 0;
			Gs = C;
			Bs = X;
		}
		else if (H >= 180 && H < 240) {
			Rs = 0;
			Gs = X;
			Bs = C;
		}
		else if (H >= 240 && H < 300) {
			Rs = X;
			Gs = 0;
			Bs = C;
		}
		else {
			Rs = C;
			Gs = 0;
			Bs = X;
		}

		return { (Rs + m), (Gs + m), (Bs + m), 1.0f };
	}
}

