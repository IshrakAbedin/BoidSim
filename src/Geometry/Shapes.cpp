#include "Shapes.h"

#include "Curves.h"

void Quad2D::Draw(Image& image) const
{
	blaze::StaticMatrix<double, 3, 4> geoMat{
		{m_Vertices[0].X, m_Vertices[1].X, m_Vertices[2].X, m_Vertices[3].X},
		{m_Vertices[0].Y, m_Vertices[1].Y, m_Vertices[2].Y, m_Vertices[3].Y},
		{			 1.0, 			  1.0, 			   1.0, 			1.0}
	};

	geoMat = GetTransformation() * geoMat;

	const Point p0{ geoMat(0, 0), geoMat(1, 0) };
	const Point p1{ geoMat(0, 1), geoMat(1, 1) };
	const Point p2{ geoMat(0, 2), geoMat(1, 2) };
	const Point p3{ geoMat(0, 3), geoMat(1, 3) };

	DrawLine(image, p0, p1, GetColor()); // Arm 1
	DrawLine(image, p1, p2, GetColor()); // Arm 2
	DrawLine(image, p2, p3, GetColor()); // Arm 3
	DrawLine(image, p3, p0, GetColor()); // Arm 4
	DrawLine(image, p0, p2, GetColor()); // Diagonal 1
	DrawLine(image, p1, p3, GetColor()); // Diagonal 2
}

Arm2D::Arm2D(double baseWidth, double height, Color color)
	: ColoredShape2D(color)
{
	m_Vertices.emplace_back(-(baseWidth / 2.0),    0.0);
	m_Vertices.emplace_back( (baseWidth / 2.0),	   0.0);
	m_Vertices.emplace_back(			   0.0, height);
}

void Arm2D::Draw(Image& image) const
{
	blaze::StaticMatrix<double, 3, 4> geoMat{
		{m_Vertices[0].X, m_Vertices[1].X, m_Vertices[2].X, 0.0},
		{m_Vertices[0].Y, m_Vertices[1].Y, m_Vertices[2].Y, 0.0},
		{			 1.0, 			  1.0, 			   1.0, 1.0}
	};

	geoMat = GetTransformation() * geoMat;

	const Point p0{ geoMat(0, 0), geoMat(1, 0) };
	const Point p1{ geoMat(0, 1), geoMat(1, 1) };
	const Point p2{ geoMat(0, 2), geoMat(1, 2) };
	const Point  c{ geoMat(0, 3), geoMat(1, 3) };
	double radius = blaze::norm(static_cast<Point2>(p0) - static_cast<Point2>(c));

	DrawLine(image, p0, p1, GetColor());		// Arm 1
	DrawLine(image, p1, p2, GetColor());		// Arm 2
	DrawLine(image, p2, p0, GetColor());		// Arm 3
	DrawCircle(image, c, radius, GetColor());	// Filled circle
}
