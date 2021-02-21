project "SympleLang"
	kind "SharedLib"
	language "C++"
	cppdialect "C++Latest"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines
	{
		"SY_x86",
		"SY_ASM",
		"SY_BUILD_DLL",
	}
	
	includedirs
	{
		"include",
	}
	
	files
	{
		"include/**.h",
		"include/**.hpp",
		
		"src/**.c",
		"src/**.cpp",
	}
	
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SympleCompiler")
	}
	
	filter "configurations:Debug"
		defines
		{
			"SY_DEBUG",
		}
		
		symbols "On"
	
	filter "configurations:Release"
		defines
		{
			"SY_RELEASE",
		}
		
		optimize "On"