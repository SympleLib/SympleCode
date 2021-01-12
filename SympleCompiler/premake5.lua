project "SympleCompiler"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines {
		"SY_32"
		-- "SY_64"
	}
	
	files {
		"src/**.h",
		"src/**.c",
		"src/**.hpp",
		"src/**.cpp"
	}
	
	includedirs {
		"src"
	}

	libdirs {}
	
	filter "configurations:Debug"
		defines "_SY_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SY_RELEASE"
		optimize "On"

	filter "configurations:Production"
		defines "SY_PRODUCTION"
		optimize "On"