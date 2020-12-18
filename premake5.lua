workspace "SympleCode"
	architecture "x86"

	configurations {
		"Debug",
		"Release",
		"Production"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SympleCompiler"