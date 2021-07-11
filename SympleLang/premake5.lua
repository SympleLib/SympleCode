project "SympleLang"
	kind "StaticLib"
	language "C++"
	cppdialect "C++Latest"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	-- pchheader "Symple/Common/Common.h"
	-- pchsource "src/Symple/Common/Common.cpp"
	
	defines "SYC_BUILD"
	
	includedirs {
		"include",
		"vendor/fmt/include",
		"vendor/llvm-project/llvm/include",
		"vendor/llvm-project/llvm/build/include",
	}
	
	files {
		"include/**.h",
		"include/**.hpp",
		
		"src/**.c",
		"src/**.cpp",
		
		"vendor/fmt/src/*.cc",
	}
	
	-- postbuildcommands
	-- {
		-- ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SympleCompiler")
	-- }
	
	filter "configurations:Debug"
		defines "SY_DEBUG"
		links "vendor/llvm-project/llvm/build/Debug/lib/LLVMCore"
		
		symbols "On"
	
	filter "configurations:Release"
		defines "SY_RELEASE"
		links "vendor/llvm-project/llvm/build/Release/lib/LLVMCore"
		
		optimize "On"