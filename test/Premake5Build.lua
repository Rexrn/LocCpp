project "UnitTests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	location (path.join(repoRoot, "build/%{prj.name}"))
	targetdir (path.join(repoRoot, "bin/%{cfg.platform}/%{cfg.buildcfg}"))

	includedirs {
		-- Rexrn::LocCpp
		path.join(repoRoot, "include"),

		-- Google Test
		path.join(userConfig.deps.gtest.root, "include")
	}

	-- Link Google Test
	
	filter "configurations:Debug"
		links {
			path.join(userConfig.deps.gtest.root, "lib/%{cfg.platform}/%{cfg.buildcfg}/gtestd"),
			path.join(userConfig.deps.gtest.root, "lib/%{cfg.platform}/%{cfg.buildcfg}/gtest_maind")
		}
		

	filter "configurations:Release"
		links {
			path.join(userConfig.deps.gtest.root, "lib/%{cfg.platform}/%{cfg.buildcfg}/gtest"),
			path.join(userConfig.deps.gtest.root, "lib/%{cfg.platform}/%{cfg.buildcfg}/gtest_main")
		}

	filter {}

	files {
		-- Current project:
		"src/**.cpp"
	}