project "SympleEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines {
		"SY_32",
		
		"null=0"
	}
	
	files {
		"inc/**.h",
		"inc/**.hpp",
		
		"src/**.c",
		"src/**.cpp",
		
		"sy/**.sy",
		
		
		"vendor/glfw/include/GLFW/glfw3.h",
		"vendor/glfw/include/GLFW/glfw3native.h",
		"vendor/glfw/src/glfw_config.h",
		"vendor/glfw/src/context.c",
		"vendor/glfw/src/init.c",
		"vendor/glfw/src/input.c",
		"vendor/glfw/src/monitor.c",
		"vendor/glfw/src/vulkan.c",
		"vendor/glfw/src/window.c"
	}
	
	includedirs {
		"inc",
		
		"vendor/glfw/include"
	}
	
	filter "system:windows"
	
	filter "configurations:Debug"
		defines "__SY_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "__SY_RELEASE"
		optimize "On"