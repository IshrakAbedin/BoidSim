#pragma once

#include "Vec.h"
#include "Shapes.h"
#include "Curves.h"

namespace Boid
{
	enum class Type
	{
		ATTRACTOR, PREY, PREDATOR
	};

	struct State
	{
		Vec2 Displacement;
		Vec2 Velocity;
		Vec2 Acceleration;

		State(Vec2 displacement, Vec2 velocity, Vec2 acceleration)
			: Displacement{ displacement }, Velocity{ velocity }, Acceleration{ acceleration } { }
		State() : State(Vec2{ 0.0, 0.0 }, Vec2{ 0.0, 0.0 }, Vec2{ 0.0, 0.0 }) { }
	};

	class SequentialIDGenerator
	{
	public:
		static unsigned long long Next()
		{
			static unsigned long long CurrentID{ 0u };
			return CurrentID++;
		}
	};

	class Entity : public ColoredShape2D // Pure Virtual Class
	{
	private:
		unsigned long long m_ID;
		Type m_Type;
		State m_CurrentState;
		State m_NextState;
		double m_MaxVelocity;

	public:
		Entity(Type type, State initialState, double maxVelocity, Color color)
			: m_ID{ SequentialIDGenerator::Next() }, m_Type{ type },
			m_CurrentState{ initialState }, m_MaxVelocity{ maxVelocity }, ColoredShape2D(color) { }

		inline bool Equals(const Entity& other) { return m_ID == other.m_ID; }
		inline bool operator==(const Entity& other) { return Equals(other); }
		inline bool operator!=(const Entity& other) { return !Equals(other); }

		inline Type& EntityType() { return m_Type; }
		inline State& CurrentState() { return m_CurrentState; }
		inline State& NextState() { return m_NextState; }

		inline const Type& EntityType() const { return m_Type; }
		inline const State& CurrentState() const { return m_CurrentState; }
		inline const State& NextState() const { return m_NextState; }

		virtual void UpdateNextState(Vec2 acceleration, double timeStep = 1.0);
		void SetNextToCurrentState() { m_CurrentState = m_NextState; }
		virtual Vec2 ProvideAcceleration(const Entity& other) const = 0;
		virtual void Draw(Image& image) const override = 0;
	};

	class Attractor : public Entity
	{
	private:
		double m_PreyAttractionScale;
	public:
		Attractor(State initialState, double preyAttractionScale = 10.0,
			double maxVelocity = 50.0, Color color = NamedColors::BloodRed)
			: Entity(Type::ATTRACTOR, initialState, maxVelocity, color),
			m_PreyAttractionScale{ preyAttractionScale } { }

		virtual Vec2 ProvideAcceleration(const Entity& other) const override;
		virtual void Draw(Image& image) const override;
	};

	class Prey : public Entity
	{
	private:
		double m_PredatorAttractionScale;
		double m_PreyRepelScale;
	public:
		Prey(State initialState, double predatorAttractionScale = 12.0, double preyRepelScale = 0.5,
			double maxVelocity = 25.0, Color color = NamedColors::FrenchViolet)
			: Entity(Type::PREY, initialState, maxVelocity, color),
			m_PredatorAttractionScale{ predatorAttractionScale }, m_PreyRepelScale{ preyRepelScale } { }

		virtual Vec2 ProvideAcceleration(const Entity& other) const override;
		virtual void Draw(Image& image) const override;
	};

	class Predator : public Entity
	{
	private:
		double m_PreyRepelScale;
		double m_PredatorRepelScale;
	public:
		Predator(State initialState, double preyRepelScale = 1.0, double predatorRepelScale = 0.75,
			double maxVelocity = 35.0, Color color = NamedColors::Orange)
			: Entity(Type::PREDATOR, initialState, maxVelocity, color),
			m_PreyRepelScale{ preyRepelScale }, m_PredatorRepelScale{ predatorRepelScale } { }

		virtual Vec2 ProvideAcceleration(const Entity& other) const override;
		virtual void Draw(Image& image) const override;
	};

	class Simulator
	{
	private:
		double m_TimeStep;
		std::vector<std::shared_ptr<Entity>> m_Entities;
	public:
		Simulator(double timeStep = 1.0) : m_TimeStep{ timeStep } { }
		inline void AddEntity(std::shared_ptr<Entity> entity) { m_Entities.push_back(entity); }

		void SimulateNext();
		void SetNextToCurrentState();
		void Draw(Image& image) const;
	};
}
