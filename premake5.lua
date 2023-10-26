-- premake5.lua
workspace "Lab4"
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
        "./src/include/stb"
    }

   files { "**.h", "**.hpp", "**.cpp", "**.cc", "**.cx", "**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"