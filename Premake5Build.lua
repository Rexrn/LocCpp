repoRoot = os.getcwd()

getWindowsSDKVersion = function()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

include ("BuildConfig.lua")

workspace "Rexrn"
	location "build"
	platforms { "x86", "x64" }
	configurations { "Debug", "Release" }

	if os.host() == "macosx" then
		removeplatforms { "x86" }
	end

	-- MSBuild specific configuration
	if string.match(_ACTION, "vs%d%d%d%d") ~= nil then
		filter {"system:windows", "action:vs*"}
    		systemversion(getWindowsSDKVersion() .. ".0")
	end

	filter "platforms:*32"
		architecture "x86"

	filter "platforms:*64"
		architecture "x86_64"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter {}

	-- The GTAOnlineSA project:
	project "LocCpp"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"

		location (path.join(repoRoot, "build/%{prj.name}"))
		targetdir (path.join(repoRoot, "bin/%{cfg.platform}/%{cfg.buildcfg}"))

		includedirs {
			"include"
		}

		files {
			-- Current project:
			"include/Rexrn/LocCpp/**.hpp",
			"include/Rexrn/LocCpp/**.inl",
			"src/**.cpp"

			-- TODO: use safer way: manually add every file.
			-- For current development, it is quicker to add them automatically.
		}


	-- Build test?
	if userConfig.build.test then
		include ("test/Premake5Build.lua")
	end

	-- Build examples?
	if userConfig.build.examples then
		include ("example/Premake5Build.lua")
	end

	