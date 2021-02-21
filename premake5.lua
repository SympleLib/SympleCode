workspace "SympleCode"
	architecture "x86"

	configurations {
		"Debug",
		"Release"
	}
	
	flags "MultiProcessorCompile"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SympleCompiler"