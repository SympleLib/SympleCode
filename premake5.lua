workspace "SympleCode"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}
	
	flags "MultiProcessorCompile"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SympleLang"
include "SympleCompiler"