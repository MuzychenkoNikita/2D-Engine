-- premake5.lua
workspace "2D-Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Game"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
      
   filter "system:macosx"
      architecture "ARM64"
      system "macosx"
   filter {}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine"
	include "Engine/Build-Engine.lua"
	
group ""
	include "Game/Build-Game.lua"