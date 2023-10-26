#pragma once

#include "blaze/Math.h"

using Vec2   = blaze::StaticVector<double, 2>;
using Vec3   = blaze::StaticVector<double, 3>;
using Point2 = blaze::StaticVector<double, 2>;
using Point3 = blaze::StaticVector<double, 3>;

struct Point
{
    double X;
    double Y;

    inline Point(double x, double y) : X{x}, Y{y} {}
    inline Point() : Point{0.0, 0.0} {}
    inline Point(const Point2& other) : Point{ other[0], other[1] } {}
    inline operator Point2() const { return Point2{ X, Y }; }
};

inline Vec2 UnitVector2(const Vec2& vec)
{
    return vec / blaze::length(vec);
}

inline void ClampVector2(Vec2& vec, double targetMagnitude)
{
    double magnitude = blaze::length(vec);
    if (magnitude > targetMagnitude)
    {
        Vec2 direction = UnitVector2(vec);
        vec = direction * targetMagnitude;
    }
}

inline Vec3 UnitVector(const Vec3& vec)
{
	return vec / blaze::length(vec);
}

inline Vec3 Reflect(const Vec3& vec, const Vec3& normal)
{
	return vec - 2 * ( blaze::dot(vec, normal) * normal );
}