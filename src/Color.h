#pragma once

#include "blaze/Math.h"
#include "Util.h"

using Color3 = blaze::StaticVector<double, 3>;

struct Color
{
	union
	{
		uint8_t V[3];
		struct {
			uint8_t R;
			uint8_t G;
			uint8_t B;
		};
	};

	inline Color(uint8_t r, uint8_t g, uint8_t b) : R{ r }, G{ g }, B{ b } {}
	inline Color(uint8_t v) : Color(v, v, v) {}
	inline Color() : Color(0, 0, 0) {}

	inline Color(const Color3& color3)
	{
		Color3 clampedColor = blaze::clamp(color3, 0.0, 1.0);
		R = static_cast<uint8_t>(255.999 * clampedColor[0]);
		G = static_cast<uint8_t>(255.999 * clampedColor[1]);
		B = static_cast<uint8_t>(255.999 * clampedColor[2]);
	}

	inline Color& operator=(const Color3& color3)
	{
		Color3 clampedColor = blaze::clamp(color3, 0.0, 1.0);
		R = static_cast<uint8_t>(255.999 * clampedColor[0]);
		G = static_cast<uint8_t>(255.999 * clampedColor[1]);
		B = static_cast<uint8_t>(255.999 * clampedColor[2]);
		return *this;
	}

	inline operator Color3() const
	{
		return Color3{ R / 256.0, G / 256.0, B / 256.0 };
	}

	inline Color operator*(double t) const { return Color(R * t, G * t, B * t); }
	inline Color operator+(double t) const { return Color(R + t, G + t, B + t); }
	inline Color operator+(const Color& other) const
	{
		return Color(
			Clamp(R + other.R, 0, 255),
			Clamp(G + other.G, 0, 255),
			Clamp(B + other.B, 0, 255)
		);
	}
	inline uint8_t operator[](size_t index) const { return V[index]; }
	inline uint8_t& operator[](size_t index) { return V[index]; }
};

namespace NamedColors
{
	static const Color Pink{ 250, 180, 180 };
	static const Color Blue{ 0, 0, 255 };
	static const Color Green{ 0, 255, 0 };
	static const Color Red{ 255, 0, 0 };
	static const Color BloodRed{ 136, 8, 8 };
	static const Color AlizarinCrimson{ 227, 38, 54 };
	static const Color JadeGreen{ 0, 168, 107 };
	static const Color CornflowerBlue{ 100, 149, 237 };
	static const Color SapphireBlue{ 15, 82, 186 };
	static const Color SquidInk{ 18, 24, 34 };
	static const Color Gray{ 128, 128, 128 };
	static const Color SlateGray{ 112, 128, 144 };
	static const Color Black{ 0, 0, 0 };
	static const Color White{ 255, 255, 255 };
	static const Color FrenchViolet{ 136, 6, 206 };
	static const Color Orange{ 255, 157, 0 };
}
