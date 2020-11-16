workspace "SympleCode"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Production"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SympleCompiler"
-- include "SympleRuntime"