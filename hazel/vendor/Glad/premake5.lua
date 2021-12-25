project "Glad"
  kind "StaticLib"
  language "C"
  staticruntime "on"

  targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
  objdir ("_obj/" .. outputdir .. "/%{prj.name}")

  files
  {
    "include/glad/glad.h",
    "include/KHR/khrplatform.h",
    "src/glad.c",
  }
  
  includedirs
  {
    "include"
  }

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  }

  filter "system:linux"
    pic "on"

    systemversion "latest"

  filter "system:windows"
    systemversion "latest"

  filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    runtime "Release"
    optimize "on"
