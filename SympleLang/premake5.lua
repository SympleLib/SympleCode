project "SympleLang"
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
		"inc/**.h",
		"inc/**.hpp",
		
		"src/**.c",
		"src/**.cpp",
		
		"sy/**.sy"
	}
	
	includedirs {
		"inc",
		
		"vendor/spdlog/include",
		"vendor/llvm-c/include"
	}
	
	links {
		"vendor/llvm-c/lib/llvm-c"
	}
	
	disablewarnings {
		"26812"
	}
	
	filter "configurations:Debug"
		defines "__SY_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "__SY_RELEASE"
		optimize "On"