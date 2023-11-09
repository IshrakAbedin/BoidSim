#include "Boid.h"

#include <cmath>
#include <iostream>

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
		default: // No attraction or repulsion force towards other entities
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
			return (m_PreyRepulsionScale * UnitVector2(direction)) / (blaze::sqrLength(direction)); // Inverse quadratic acceleration
		case Type::PREDATOR: // Attracts predators
			direction = CurrentState().Displacement - other.CurrentState().Displacement; // Towards self
			return m_PredatorAttractionScale * UnitVector2(direction); // Constant acceleration
		default: // No attraction or repulsion force towards other entities
			return Vec2{ 0.0, 0.0 };
		}
	}

	void Prey::Draw(Image& image) const
	{
		DrawCircle(image, CurrentState().Displacement, 10.0, GetColor());
		static Vec2 upVec{ 0.0, 1.0 };
		Vec2 directionVec{ 0.0, 1.0 };
		if (!blaze::isZero(CurrentState().Velocity))
			directionVec = UnitVector2(CurrentState().Velocity);
		double angle = blaze::acos(blaze::dot(upVec, directionVec));
		double sign = directionVec[0] <= 0.0 ? 1.0 : -1.0;

		double velocityRatio = Clamp((blaze::length(CurrentState().Velocity) / MaxVelocity()), 0.1, 1.0);


		Transform2D rotation{ Transform::Rotate2D(sign * angle) };
		Transform2D scale{ Transform::Scale2D(1.0, velocityRatio) };
		Transform2D translation{
			Transform::Translate2D(CurrentState().Displacement[0],
			CurrentState().Displacement[1])
		};

		m_Shape->SetTransformation(translation * rotation * scale);
		m_Shape->Draw(image);
	}

	Vec2 Predator::ProvideAcceleration(const Entity& other) const
	{
		Vec2 direction;
		switch (other.EntityType())
		{
		case Type::PREY: // Repels preys
			direction = other.CurrentState().Displacement - CurrentState().Displacement; // Away from self
			return (m_PreyRepulsionScale * UnitVector2(direction)) / (blaze::sqrLength(direction)); // Inverse quadratic acceleration
		case Type::PREDATOR: // Repels predators
			direction = other.CurrentState().Displacement - CurrentState().Displacement; // Away from self
			return (m_PredatorRepulsionScale * UnitVector2(direction)) / (blaze::length(direction)); // Inverse linear acceleration
		default: // No attraction or repulsion force towards other entities
			return Vec2{ 0.0, 0.0 };
		}
	}

	void Predator::Draw(Image& image) const
	{
		DrawCircle(image, CurrentState().Displacement, 15.0, GetColor());
		static Vec2 upVec{ 0.0, 1.0 };
		Vec2 directionVec{ 0.0, 1.0 };
		if (!blaze::isZero(CurrentState().Velocity))
			directionVec = UnitVector2(CurrentState().Velocity);
		double angle = blaze::acos(blaze::dot(upVec, directionVec));
		double sign = directionVec[0] <= 0.0 ? 1.0 : -1.0;

		double velocityRatio = Clamp((blaze::length(CurrentState().Velocity) / MaxVelocity()), 0.1, 1.0);


		Transform2D rotation{ Transform::Rotate2D(sign * angle) };
		Transform2D scale{ Transform::Scale2D(1.0, velocityRatio) };
		Transform2D translation{
			Transform::Translate2D(CurrentState().Displacement[0],
			CurrentState().Displacement[1])
		};

		m_Shape->SetTransformation(translation * rotation * scale);
		m_Shape->Draw(image);
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

	Point BezierDrivenADS::NextPoint()
	{
		float integralIncrement;
		float fractionalIncrement = std::modf(m_CurrentAmount, &integralIncrement);
		size_t curveIndex = static_cast<size_t>(integralIncrement) % m_ControlPointSets.size();
		Point p = GetPointOnBezierCurve(m_ControlPointSets[curveIndex], fractionalIncrement);
		m_CurrentAmount += m_IncrementAmount;
		return p;
	}

	void SystemDrivenAttractor::UpdateNextState(Vec2 acceleration, double timeStep)
	{
		NextState().Displacement = m_ADS->NextPoint();
	}
}
