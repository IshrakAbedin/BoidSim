<!-- Written by Mohammad Ishrak Abedin-->
# BoidSim

### A simple force based simulation of Bird-oid Objects (Boids) with attractors, preys, and predators.

 The project relies on **[PREMAKE](https://premake.github.io/)** meta build system and can be compiled in Windows or Linux using a preferred build system (Expected to use MSVC/Clang with Visual Studio for Windows and GCC with GNU Make for Linux). It assumes that you have Premake 5 or higher installed and it can be directly accessed through the system environment.

## Libraries Used

+ [blaze-lib](https://bitbucket.org/blaze-lib/blaze/) for Matrix and Vector operations
+ [std_image_write](https://github.com/nothings/stb) for writing the rendered images

*Libraries are already included with the source.*

## Packages Required for Linux

+ GCC
+ Make
+ Premake 5.0


**C++ 17 or higher is a must to compile since the project relies on `std::filesystem`. The project is only written and tested for x64 systems.**

## Building for Windows (MSVC and Visual Studio)

The Windows version is targeted towards MSVC/Clang using Microsoft Visual Studio. A Visual Studio version 2022 or Higher is recommended.

Run the following command to in the project root directory generate the project files for the respective version of Visual Studio.

```powershell
premake5 vs2022 # Pass respective Visual Studio version
```

At this point, a Visual Studio Solution (`BoidSim.sln`) will be generated. Open it with Visual Studio and build the project.


## Building for Linux (GCC and GNU Make)

Run the following command in the project root directory to generate the project files for GNU Make.

```bash
premake5 gmake2 # Generates Makefile
```

To build the application in *Release* mode, run:

```bash
make config=release # For building in Release mode
```

Or to build in *Debug* mode, run:

```bash
make config=debug # For building in Debug mode
```

## Using the Application

After building, the executable is located in `bin/Release/` or `bin/Debug/` based on the build configuration. After running the executable, it will first create a folder named `Output` if it does not exist. Then the rendered frames will be placed inside the `Output` folder named as `Image_<FrameNumber>.jpg`. The frames then can be compiled into a video using an appropriate application (for instance, [`FFmpeg`](https://www.ffmpeg.org/) can used as a cross platform commandline tool).


## Tuning Parameters

Most of the major parameters for the simulation are set from `constexpr` values or preprocessor constants located at the beginning of [`src/Application.cpp`](./src/Application.cpp) file. The values may be tuned to change the behavior of the simulation. Following is a snippet with the parameters:

```C++
// Application.cpp
// ... ...
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
// ... ...
```
