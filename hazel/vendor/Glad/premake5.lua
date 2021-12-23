project "Glad"
  kind "StaticLib"
  language "C"

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

  filter "system:linux"
    pic "On"

    systemversion "latest"
    staticruntime "On"

  filter "system:windows"
    systemversion "latest"
    staticruntime "On"