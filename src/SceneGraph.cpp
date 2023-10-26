#include "SceneGraph.h"

void SceneNode::AddChildren(std::shared_ptr<SceneNode> child)
{
	// The following shared pointer with special deletion is required to avoid
	// circular dependency of shared pointers
	child->m_Parent = std::shared_ptr<SceneNode>(this, [](SceneNode*) {});
	m_Children.push_back(child);
}

void SceneNode::UpdateModelMatrix()
{
	if (m_Parent == nullptr)
		m_ModelMatrix = m_Transformation;
	else
		m_ModelMatrix = m_Parent->m_ModelMatrix * m_Transformation;
}

void SceneNode::Draw(Image& image, bool update)
{
	if (update) UpdateModelMatrix();
	if (m_Shape != nullptr)
	{
		m_Shape->SetTransformation(m_ModelMatrix);
		m_Shape->Draw(image);
	}
	for (const auto& child : m_Children)
		child->Draw(image, update);
}
