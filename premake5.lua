-- premake5.lua
workspace "BoidSim"
   configurations { "Debug", "Release" }
   
   project "Boid"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   architecture "x64"
   targetdir "bin/%{cfg.buildcfg}"

   includedirs {
        "./src",
        "./src/include",
        "./src/include/stb",
        "./src/Boid",
        "./src/Geometry",
        "./src/Image",
        "./src/MathUtil",
        "./src/Scene"
    }

   files { "**.h", "**.hpp", "**.cpp", "**.cc", "**.cx", "**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"