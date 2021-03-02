project "SympleLang"
	kind "StaticLib"
	language "C++"
	cppdialect "C++Latest"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	-- pchheader "Symple/Common/Common.h"
	-- pchsource "src/Symple/Common/Common.cpp"
	
	defines
	{
		"SY_x86",
		"SY_ASM",
		"SY_CODE_BUILD",
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
	
	-- postbuildcommands
	-- {
		-- ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SympleCompiler")
	-- }
	
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