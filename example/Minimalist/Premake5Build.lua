project "Minimalist"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	location (path.join(repoRoot, "build/%{prj.name}/examples"))
	targetdir (path.join(repoRoot, "bin/%{cfg.platform}/%{cfg.buildcfg}/examples"))

	includedirs {
		-- Rexrn::LocCpp
		path.join(repoRoot, "include"),
	}

	files {
		-- Current project:
		"src/**.cpp"
	}