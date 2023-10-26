#pragma once

#include "blaze/Math.h"

using Transform2D = blaze::StaticMatrix<double, 3U, 3U>;

namespace Transform
{
	// Identity matrix for 2D homogenious transformations
	const Transform2D Identity2D = Transform2D{
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0}
	};
	Transform2D Scale2D(double sx, double sy); // Scale in X and Y direction respectively
	Transform2D Rotate2D(double theta); // Angle in radian (counter-clockwise is positive)
	Transform2D Translate2D(double dx, double dy); // Translation in X and Y direction respectively
}