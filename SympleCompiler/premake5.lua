project "SympleCompiler"
	kind "SharedLib"
	language "C++"
	cppdialect "C++Latest"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines
	{
		"SY_x86",
		"SY_ASM",
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