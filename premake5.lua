workspace "Hazel"
  architecture "x86_64"
  startproject "Sandbox" -- Visual Studio only

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

-- See: https://premake.github.io/docs/Tokens/
-- e.g. Debug-Windows-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "hazel/vendor/Glad/include"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"

project "Hazel"
  location "hazel"
  kind "SharedLib"
  language "C++"

  targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
  objdir ("_obj/" .. outputdir .. "/%{prj.name}")

  pchheader "hzpch.h"
  pchsource "%{prj.name}/src/hzpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}"
  }

  links 
  { 
    "GLFW",
    "Glad",
    "opengl32.lib"
  }

  postbuildcommands
  {
    ("{COPY} %{cfg.buildtarget.relpath} ../_bin/" .. outputdir .. "/Sandbox")
  }

  filter "system:windows"
    cppdialect "C++17"
    systemversion "latest" -- 10.0.19041.0

    defines
    {
      "HZ_PLATFORM_WINDOWS",
      "HZ_BUILD_DLL",
      "GLFW_INCLUDE_NONE"
    }

  filter { "configurations:Debug", "system:windows" }
    defines {
      "HZ_DEBUG",
      "HZ_ENABLE_ASSERTS"
    }
    buildoptions { "/MDd" }
    symbols "On"

  filter { "configurations:Release", "system:windows" }
    defines "HZ_RELEASE"
    buildoptions { "/MD" }
    optimize "On"

  filter { "configurations:Dist", "system:windows" }
    defines "HZ_DIST"
    buildoptions { "/MD" }
    optimize "On"

project "Sandbox"
  location "sandbox"
  kind "ConsoleApp"
  language "C++"

  targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
  objdir ("_obj/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
  }

  includedirs
  {
    "hazel/src",
    "hazel/vendor/spdlog/include"
  }

  links
  {
    "Hazel"
  }

  filter "system:windows"
    cppdialect "C++17"
    systemversion "latest" -- 10.0.19041.0

    defines "HZ_PLATFORM_WINDOWS"

  filter { "configurations:Debug", "system:windows" }
    defines "HZ_DEBUG"
    buildoptions { "/MDd" }
    symbols "On"

  filter { "configurations:Release", "system:windows" }
    defines "HZ_RELEASE"
    buildoptions { "/MD" }
    optimize "On"

  filter { "configurations:Dist", "system:windows" }
    defines "HZ_DIST"
    buildoptions { "/MDd" }
    optimize "On"