#include "Boid.h"
#include "Util.h"


namespace Boid
{
	void Entity::UpdateNextState(Vec2 acceleration, double timeStep)
	{
		NextState().Acceleration = acceleration; // No interpolation for smoothness/intertia
		NextState().Velocity = CurrentState().Velocity + (CurrentState().Acceleration * timeStep);
		ClampVector2(NextState().Velocity, m_MaxVelocity);
		NextState().Displacement = CurrentState().Displacement + (CurrentState().Velocity * timeStep);
	}

	Vec2 Attractor::ProvideAcceleration(const Entity& other) const
	{
		Vec2 direction;
		switch (other.EntityType())
		{
		case Type::PREY: // Only attracts preys
			direction = CurrentState().Displacement - other.CurrentState().Displacement; // Towards self
			return m_PreyAttractionScale * UnitVector2(direction); // Constant acceleration
		default: // No attraction or repel force towards other entities
			return Vec2{ 0.0, 0.0 };
		}
	}

	void Attractor::Draw(Image& image) const
	{
		DrawCircle(image, CurrentState().Displacement, 5.0, GetColor());
	}

	Vec2 Prey::ProvideAcceleration(const Entity& other) const
	{
		Vec2 direction;
		switch (other.EntityType())
		{
		case Type::PREY: // Repels other preys
			direction = other.CurrentState().Displacement - CurrentState().Displacement; // Away from self
			return m_PreyRepelScale / (direction * direction); // Inverse quadratic acceleration
		case Type::PREDATOR: // Attracts predators
			direction = CurrentState().Displacement - other.CurrentState().Displacement; // Towards self
			return m_PredatorAttractionScale * UnitVector2(direction); // Constant acceleration
		default: // No attraction or repel force towards other entities
			return Vec2{ 0.0, 0.0 };
		}
	}

	void Prey::Draw(Image& image) const
	{
		DrawCircle(image, CurrentState().Displacement, 10.0, GetColor());
	}

	Vec2 Predator::ProvideAcceleration(const Entity& other) const
	{
		Vec2 direction;
		switch (other.EntityType())
		{
		case Type::PREY: // Repels preys
			direction = other.CurrentState().Displacement - CurrentState().Displacement; // Away from self
			return m_PreyRepelScale / (direction * direction); // Inverse quadratic acceleration
		case Type::PREDATOR: // Repels predators
			direction = other.CurrentState().Displacement - CurrentState().Displacement; // Away from self
			return m_PredatorRepelScale / (direction); // Inverse quadratic acceleration
		default: // No attraction or repel force towards other entities
			return Vec2{ 0.0, 0.0 };
		}
	}

	void Predator::Draw(Image& image) const
	{
		DrawCircle(image, CurrentState().Displacement, 15.0, GetColor());
	}

	void Simulator::SimulateNext()
	{
		for (auto& currentEntity : m_Entities)
		{
			Vec2 totalAcceleration{ 0.0, 0.0 };
			for (const auto& otherEntity : m_Entities)
			{
				if ((*currentEntity) != (*otherEntity))
				{
					totalAcceleration += otherEntity->ProvideAcceleration(*(currentEntity));
				}
			}
			currentEntity->UpdateNextState(totalAcceleration, m_TimeStep);
		}
	}

	void Simulator::SetNextToCurrentState()
	{
		for (auto& entity : m_Entities)
		{
			entity->SetNextToCurrentState();
		}
	}

	void Simulator::Draw(Image& image) const
	{
		for (const auto& entity : m_Entities)
		{
			entity->Draw(image);
		}
	}
}

