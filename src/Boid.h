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

	protected:
		inline Type& EntityType() { return m_Type; }
		inline State& CurrentState() { return m_CurrentState; }
		inline State& NextState() { return m_NextState; }
		inline double& MaxVelocity() { return m_MaxVelocity; }

	public:
		Entity(Type type, State initialState, double maxVelocity, Color color)
			: m_ID{ SequentialIDGenerator::Next() }, m_Type{ type },
			m_CurrentState{ initialState }, m_MaxVelocity{ maxVelocity }, ColoredShape2D(color) { }

		inline bool Equals(const Entity& other) { return m_ID == other.m_ID; }
		inline bool operator==(const Entity& other) { return Equals(other); }
		inline bool operator!=(const Entity& other) { return !Equals(other); }

		inline const Type& EntityType() const { return m_Type; }
		inline const State& CurrentState() const { return m_CurrentState; }
		inline const State& NextState() const { return m_NextState; }
		inline const double& MaxVelocity() const { return m_MaxVelocity; }

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
		std::shared_ptr<Arm2D> m_Shape;
	public:
		Prey(State initialState, double predatorAttractionScale = 12.0, double preyRepelScale = 0.5,
			double maxVelocity = 25.0, Color color = NamedColors::FrenchViolet)
			: Entity(Type::PREY, initialState, maxVelocity, color),
			m_PredatorAttractionScale{ predatorAttractionScale }, m_PreyRepelScale{ preyRepelScale }
		{
			m_Shape = std::make_shared<Arm2D>(20.0, 30.0, color);
		}

		virtual Vec2 ProvideAcceleration(const Entity& other) const override;
		virtual void Draw(Image& image) const override;
	};

	class Predator : public Entity
	{
	private:
		double m_PreyRepelScale;
		double m_PredatorRepelScale;
		std::shared_ptr<Arm2D> m_Shape;
	public:
		Predator(State initialState, double preyRepelScale = 1.0, double predatorRepelScale = 0.75,
			double maxVelocity = 35.0, Color color = NamedColors::Orange)
			: Entity(Type::PREDATOR, initialState, maxVelocity, color),
			m_PreyRepelScale{ preyRepelScale }, m_PredatorRepelScale{ predatorRepelScale }
		{
			m_Shape = std::make_shared<Arm2D>(30.0, 45.0, color);
		}

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

	// MakeShift: Should be converted to something modular later
	class AttractorRotationSytem
	{
	private:
		std::vector<Point> m_ControlPointsSet0;
		std::vector<Point> m_ControlPointsSet1;
		double m_IncrementAmount;
		double m_CurrentAmount;

	public:
		AttractorRotationSytem(const std::vector<Point>& cp0,
			const std::vector<Point>& cp1, double incrementAmount)
			: m_ControlPointsSet0{ cp0 }, m_ControlPointsSet1{ cp1 },
			m_IncrementAmount{ incrementAmount }, m_CurrentAmount{ 0.0 } { }

		Point NextPoint();
	};

	// A Makeshift class that will be driven using an AttractorRotationSystem
	class SystemDrivenAttractor : public Attractor
	{
	private:
		AttractorRotationSytem m_ARS;
	public:
		SystemDrivenAttractor(const AttractorRotationSytem& ars,
			double preyAttractionScale = 10.0, double maxVelocity = 50.0,
			Color color = NamedColors::BloodRed)
			: m_ARS{ ars }, Attractor(State{}, preyAttractionScale, maxVelocity, color)
		{
			CurrentState().Displacement = m_ARS.NextPoint();
		}

		virtual void UpdateNextState(Vec2 acceleration, double timeStep = 1.0) override;
	};
}
