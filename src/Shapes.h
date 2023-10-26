#pragma once

#include "Transformation.h"
#include "Image.h"

class Shape2D
{
private:
	Transform2D m_Transformation;
public:
	Shape2D() : m_Transformation{Transform::Identity2D} { }
	virtual ~Shape2D() { }

	inline void SetTransformation(Transform2D transformation) { m_Transformation = transformation; }
	inline Transform2D GetTransformation() const { return m_Transformation; }

	inline virtual void Draw(Image& image) const { }
};

class ColoredShape2D : public Shape2D
{
private:
	Color m_Color;
public:
	ColoredShape2D(Color color) : Shape2D(), m_Color{ color } { }
	ColoredShape2D() : ColoredShape2D(NamedColors::Black) { }

	inline void SetColor(Color color) { m_Color = color; }
	inline Color GetColor() const { return m_Color; }

	inline virtual void Draw(Image& image) const override { }
};

class Quad2D : public ColoredShape2D
{
private:
	std::vector<Point> m_Vertices;
public:
	Quad2D(const std::vector<Point>& vertices, Color color) : ColoredShape2D(color), m_Vertices{vertices} { }
	Quad2D(const std::vector<Point>& vertices) : Quad2D(vertices, NamedColors::Blue) { }

	virtual void Draw(Image& image) const override;
};

class Arm2D : public ColoredShape2D
{
private:
	std::vector<Point> m_Vertices;
public:
	Arm2D(double baseWidth, double height, Color color);
	Arm2D(double baseWidth, double height) : Arm2D(baseWidth, height, NamedColors::Blue) { }

	virtual void Draw(Image& image) const override;
};