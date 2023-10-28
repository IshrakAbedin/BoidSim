#include <iostream>
#include <chrono>

#include "Boid.h"
#include "Util.h"

constexpr size_t FRAME_COUNT = 3000u;
constexpr double TIME_STEP = 0.1;

constexpr double ATTRACTOR_PRAY_ATTRACTION_SCALE = 10.0;
constexpr double ATTRACTOR_MAX_VELOCITY = 50.0;
constexpr double BEZIER_ADS_CYCLE_COUNT = 3.0;

constexpr size_t PREY_COUNT = 10u;
constexpr double PREY_CENTROID_X = 700.0;
constexpr double PREY_CENTROID_Y = 700.0;
constexpr double PREY_DEVIATION_X = 200.0;
constexpr double PREY_DEVIATION_Y = 200.0;
constexpr double PREY_PREDATOR_ATTRACTION_SCALE = 12.0;
constexpr double PREY_PREY_REPEL_SCALE = 0.5;
constexpr double PREY_MAX_VELOCITY = 35.0;

constexpr size_t PREDATOR_COUNT = 3u;
constexpr double PREDATOR_CENTROID_X = 500.0;
constexpr double PREDATOR_CENTROID_Y = 500.0;
constexpr double PREDATOR_DEVIATION_X = 450.0;
constexpr double PREDATOR_DEVIATION_Y = 450.0;
constexpr double PREDATOR_PREY_REPEL_SCALE = 10.0;
constexpr double PREDATOR_PREDATOR_REPEL_SCALE = 25.0;
constexpr double PREDATOR_MAX_VELOCITY = 45.0;

// A simple struct to hold and convert time from total seconds to hour, minutes, and seconds
struct SimpleFormattedTime
{
	unsigned int Hours;
	unsigned int Minutes;
	double Seconds;

	SimpleFormattedTime(double totalSeconds)
	{
		Hours = static_cast<unsigned int>(totalSeconds / (3600.0));
		totalSeconds -= (Hours * 3600.0);
		Minutes = static_cast<unsigned int>(totalSeconds / (60.0));
		totalSeconds -= (Minutes * 60.0);
		Seconds = totalSeconds;
	}
};

void PrintSimulationStatus(size_t currentFrameCount, size_t totalFrameCount,
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime);

void PrintCompletionTime(std::chrono::time_point<std::chrono::high_resolution_clock> startTime);

int main()
{
	char WritePath[1024]; // Will be used for C style string formatting as C++17 does not have required standard lib
	Boid::Simulator boidSim{ TIME_STEP };

	// Add an attractor with Attractor Driving System (ADS)
	std::shared_ptr<Boid::iAttractorDrivingSystem> ADS{
		std::make_shared<Boid::BezierDrivenADS>
		(
			std::vector<std::vector<Point>>{
				{ {31, 510}, {105, 1200}, {693, 1023}, {1020, 402} },
				{ {1020, 402 }, {696, 0}, {33, 120}, {31, 510} },
				{ {31, 510 }, {333, 510}, {680, 510}, {1020, 510} },
				{ {1020, 510 }, {800, 810}, {600, 310}, {512, 512} }
			},
			BEZIER_ADS_CYCLE_COUNT / FRAME_COUNT
		)
	};

	boidSim.AddEntity(
		std::make_shared<Boid::SystemDrivenAttractor>(
			ADS,
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
	const auto startTime = std::chrono::high_resolution_clock::now();
	for (size_t i = 0u; i < FRAME_COUNT; i++)
	{
		boidSim.SimulateNext();
		boidSim.Draw(image);
		boidSim.SetNextToCurrentState();

		std::sprintf(WritePath, "./Output/Image_%.4zu.png", i);
		image.Write(WritePath);
		image.Clear();
		
		PrintSimulationStatus(i + 1, FRAME_COUNT, startTime);
	}
	std::cout << std::endl;
	PrintCompletionTime(startTime);

	return 0;
}

void PrintSimulationStatus(size_t currentFrameCount, size_t totalFrameCount,
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime)
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto timeDifference = currentTime - startTime;
	auto unitTime = timeDifference / currentFrameCount;
	auto requiredTime = unitTime * (totalFrameCount - currentFrameCount);
	double completionPercentage = (static_cast<double>(currentFrameCount) / totalFrameCount) * 100.0;
	double secondCount = std::chrono::duration_cast<std::chrono::milliseconds>(requiredTime).count() / 1000.0;
	SimpleFormattedTime remainingTime{ secondCount };
	std::printf("Completed %zu/%zu frames [%5.2lf%%] :: Estimated Time Remaining: %02u:%02u:%05.2lf\r",
		currentFrameCount, totalFrameCount, completionPercentage, remainingTime.Hours,
		remainingTime.Minutes, remainingTime.Seconds);
}

void PrintCompletionTime(std::chrono::time_point<std::chrono::high_resolution_clock> startTime)
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto elapsedSeconds =
		std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0;
	SimpleFormattedTime completionTime{ static_cast<double>(elapsedSeconds) };
	std::printf("Completion Time: %02u:%02u:%05.2lf\n",
		completionTime.Hours, completionTime.Minutes, completionTime.Seconds);
}
