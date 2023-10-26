#pragma once

#include <vector>
#include "Image.h"

constexpr int TANGENT_VISLEN_DIVISOR = 3;

enum class CurveType
{
    HERMITE, BEZIER, BSPLINE
};

Image& DrawCircle(Image& image, const Point& point, int radius, const Color& color);
static Image& DrawLineLow(Image& image, const Point& p0, const Point& p1, const Color& color);
static Image& DrawLineHigh(Image& image, const Point& p0, const Point& p1, const Color& color);
Image& DrawLine(Image& image, const Point& p0, const Point& p1, const Color& color = NamedColors::BloodRed);
Image& DrawHermiteCurve(Image& image, const std::vector<Point>& points, const Color& color, double samplingInterval = 0.001);
Image& DrawBezierCurve(Image& image, const std::vector<Point>& points, const Color& color, double samplingInterval = 0.001);
Image& DrawBSplineSegment(Image& image, const std::vector<Point>& points, const Color& color, double samplingInterval = 0.001);
Image& DrawBSplineCurve(Image& image, const std::vector<Point>& points, const Color& color, double samplingInterval = 0.001);
Image& DrawCurve(Image& image, CurveType curveType, const std::vector<Point>& points, int circleRadius = 5,
    const Color& curveColor = NamedColors::SapphireBlue, const Color& lineColor = NamedColors::BloodRed,
    const Color& tangentColor = NamedColors::JadeGreen, const Color& circleColor = NamedColors::AlizarinCrimson,
    double samplingInterval = 0.001);