#include <iostream>

#include "Boid.h"
#include "Util.h"

constexpr size_t FRAME_COUNT = 300;
constexpr double TIME_STEP = 0.5;

constexpr double ATTRACTOR_PRAY_ATTRACTION_SCALE = 10.0;
constexpr double ATTRACTOR_MAX_VELOCITY = 50.0;

constexpr size_t PREY_COUNT = 10;
constexpr double PREY_CENTROID_X = 700.0;
constexpr double PREY_CENTROID_Y = 700.0;
constexpr double PREY_DEVIATION_X = 200.0;
constexpr double PREY_DEVIATION_Y = 200.0;
constexpr double PREY_PREDATOR_ATTRACTION_SCALE = 12.0;
constexpr double PREY_PREY_REPEL_SCALE = 0.5;
constexpr double PREY_MAX_VELOCITY = 35.0;

constexpr size_t PREDATOR_COUNT = 3;
constexpr double PREDATOR_CENTROID_X = 500.0;
constexpr double PREDATOR_CENTROID_Y = 500.0;
constexpr double PREDATOR_DEVIATION_X = 450.0;
constexpr double PREDATOR_DEVIATION_Y = 450.0;
constexpr double PREDATOR_PREY_REPEL_SCALE = 10.0;
constexpr double PREDATOR_PREDATOR_REPEL_SCALE = 25.0;
constexpr double PREDATOR_MAX_VELOCITY = 45.0;


int main()
{
	char WritePath[1024]; // Will be used for C style string formatting as C++17 does not have required standard lib
	Boid::Simulator boidSim{ TIME_STEP };

	// Add an attractor
	boidSim.AddEntity(
		std::make_shared<Boid::Attractor>(
			Boid::State{
				Vec2{ 500.0, 500.0 },
				Vec2{ 0.0, 0.0 },
				Vec2{ 0.0, 0.0 },
			},
			ATTRACTOR_PRAY_ATTRACTION_SCALE,
			ATTRACTOR_MAX_VELOCITY
		)
	);

	// Add preys 
	for (size_t i = 0u; i < PREY_COUNT; i++)
	{
		boidSim.AddEntity(
			std::make_shared<Boid::Prey>(
				Boid::State{
					Vec2{ PREY_CENTROID_X + RandomDouble(-PREY_DEVIATION_X ,PREY_DEVIATION_X),
					PREY_CENTROID_Y + RandomDouble(-PREY_DEVIATION_Y ,PREY_DEVIATION_Y)},
					Vec2{ 0.0, 0.0 },
					Vec2{ 0.0, 0.0 }
				},
				PREY_PREDATOR_ATTRACTION_SCALE,
				PREY_PREY_REPEL_SCALE,
				PREY_MAX_VELOCITY
			)
		);
	}

	// Add predators
	for (size_t i = 0u; i < PREDATOR_COUNT; i++)
	{
		boidSim.AddEntity(
			std::make_shared<Boid::Predator>(
				Boid::State{
					Vec2{ PREDATOR_CENTROID_X + RandomDouble(-PREDATOR_DEVIATION_X, PREDATOR_DEVIATION_X),
					PREDATOR_CENTROID_Y + RandomDouble(-PREDATOR_DEVIATION_Y ,PREDATOR_DEVIATION_Y)},
					Vec2{ 0.0, 0.0 },
					Vec2{ 0.0, 0.0 }
				},
				PREDATOR_PREY_REPEL_SCALE,
				PREDATOR_PREDATOR_REPEL_SCALE,
				PREDATOR_MAX_VELOCITY
			)
		);
	}


	// Simulate frames
	Image image{ 1024, 1024, NamedColors::Black };
	for (size_t i = 0u; i < FRAME_COUNT; i++)
	{
		boidSim.SimulateNext();
		boidSim.Draw(image);
		boidSim.SetNextToCurrentState();

		std::sprintf(WritePath, "./Output/Image_%.4zu.png", i);
		image.Write(WritePath);
		image.Clear();
		std::cout << "Completed Iteration: " << i << std::endl;
	}
	return 0;
}

