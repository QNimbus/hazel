workspace "Hazel"
  architecture "x86_64"
  startproject "Sandbox" -- Visual Studio only

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

  flags
  {
    "MultiProcessorCompile"
  }

-- See: https://premake.github.io/docs/Tokens/
-- e.g. Debug-Windows-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "hazel/vendor/imgui"
IncludeDir["glm"] = "hazel/vendor/glm"
IncludeDir["stb_image"] = "hazel/vendor/stb_image"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"
  location "hazel"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

  targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
  objdir ("_obj/" .. outputdir .. "/%{prj.name}")

  pchheader "hzpch.h"
  pchsource "%{prj.name}/src/hzpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/vendor/glm/glm/**.hpp",
    "%{prj.name}/vendor/glm/glm/**.inl",
    "%{prj.name}/vendor/stb_image/**.h",
    "%{prj.name}/vendor/stb_image/**.cpp"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.stb_image}"
  }

  links 
  { 
    "GLFW",
    "Glad",
    "ImGui",
    "opengl32.lib"
  }

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  }

  filter "system:windows"
    systemversion "latest" -- 10.0.19041.0

    defines
    {
      "HZ_PLATFORM_WINDOWS",
      "HZ_BUILD_DLL",
      "GLFW_INCLUDE_NONE"
    }

  filter { "configurations:Debug"}
    defines {
      "HZ_DEBUG"
    }
    symbols "on"
    runtime "Debug"

  filter { "configurations:Release"}
    defines "HZ_RELEASE"
    optimize "on"
    runtime "Release"

  filter { "configurations:Dist" }
    defines "HZ_DIST"
    optimize "on"
    runtime "Release"

project "Sandbox"
  location "sandbox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

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
    "hazel/vendor",
    "hazel/vendor/spdlog/include",
    "%{IncludeDir.glm}"
  }

  links
  {
    "Hazel"
  }

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  }

  filter "system:windows"
    systemversion "latest" -- 10.0.19041.0

    defines "HZ_PLATFORM_WINDOWS"

  filter { "configurations:Debug"}
    defines {
      "HZ_DEBUG"
    }
    symbols "on"
    runtime "Debug"

  filter { "configurations:Release"}
    defines "HZ_RELEASE"
    optimize "on"
    runtime "Release"

  filter { "configurations:Dist" }
    defines "HZ_DIST"
    optimize "on"
    runtime "Release"
