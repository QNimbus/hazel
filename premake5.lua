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

include "Hazel/vendor/GLFW"

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
    "%{IncludeDir.GLFW}"
  }

  links 
  { 
    "GLFW",
    "opengl32.lib"
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest" -- 10.0.19041.0

    defines
    {
      "HZ_PLATFORM_WINDOWS",
      "HZ_BUILD_DLL"
    }

    postbuildcommands
    {
      ("{COPYFILE} %{cfg.buildtarget.relpath} ../_bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
    defines {
      "HZ_DEBUG",
      "HZ_ENABLE_ASSERTS"
    }
    symbols "On"

  filter "configurations:Release"
    defines "HZ_RELEASE"
    optimize "On"

  filter "configurations:Debug"
    defines "HZ_DIST"
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
    staticruntime "On"
    systemversion "latest" -- 10.0.19041.0

    defines "HZ_PLATFORM_WINDOWS"

  filter "configurations:Debug"
    defines "HZ_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "HZ_RELEASE"
    optimize "On"

  filter "configurations:Debug"
    defines "HZ_DIST"
    optimize "On"