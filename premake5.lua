workspace "SympleCode"
	architecture "x86"

	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SympleLang"
include "SympleCompiler"