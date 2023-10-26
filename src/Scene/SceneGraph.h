#pragma once

#include <memory>

#include "Shapes.h"

// Can inherit SceneNode from Shape2D and turn it into
// complete "Composite Pattern" based Implementation
class SceneNode
{
private:
	Transform2D m_Transformation;
	std::shared_ptr<Shape2D> m_Shape;
	std::shared_ptr<SceneNode> m_Parent;
	std::vector<std::shared_ptr<SceneNode>> m_Children;

	Transform2D m_ModelMatrix;
public:
	SceneNode(std::shared_ptr<Shape2D> shape) :
		m_Shape{ shape }, m_Transformation{ Transform::Identity2D }, m_ModelMatrix{ Transform::Identity2D } { }
	SceneNode() : SceneNode(nullptr) { }

	inline void SetTransformation(Transform2D transformation) { m_Transformation = transformation; }
	inline Transform2D GetTransformation() const { return m_Transformation; }

	inline std::shared_ptr<SceneNode> Parent() { return m_Parent; }

	void AddChildren(std::shared_ptr<SceneNode> child);
	void UpdateModelMatrix();
	void Draw(Image& image, bool update = true);
};

// Written for the sake of completeness
// In real scenearios, will hold management information
class SceneGraph
{
private:
	std::shared_ptr<SceneNode> m_Root;
public:
	SceneGraph(std::shared_ptr<SceneNode> root) : m_Root{ root } { }
	SceneGraph() : m_Root{ nullptr } { }

	inline std::shared_ptr<SceneNode> Root() { return m_Root; }
	inline void Draw(Image& image, bool update = true) { if (m_Root != nullptr) m_Root->Draw(image, update); }
};