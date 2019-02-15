-- This is configuration file for build environment generation.
-- 
-- To properly configure build:
--  1) Copy BuildConfig.template.lua to new file and name it BuildConfig.lua
--  2) Fill in essential information
-- Use Lua syntax.

userConfig = {
	build = {
		test = true
	},

	-- Dependencies configuration:
	deps = {
		-- [Required if building test]
		-- Please provide path to the root of the Google Test root.
		gtest = { root = "" }
	}
}