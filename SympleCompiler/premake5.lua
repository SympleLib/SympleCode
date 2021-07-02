project "SympleCompiler"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++Latest"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"include",
		
		"%{wks.location}/SympleLang/include",
	}
	
	files
	{
		"include/**.h",
		"include/**.hpp",
		
		"src/**.c",
		"src/**.cpp",
		
		"sy/**.sy",
		"sy/**.syc",
	}
	
	links
	{
		"SympleLang",
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