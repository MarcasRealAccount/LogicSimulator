local ProjectName = "PremakeProject"

local globalApp = require("premake/app")
local app = globalApp.local_app(true)

local utils = require("premake/utils")

globalApp.premakeWorkspace(ProjectName, utils.get_platforms(), { "Debug", "Release" }, app, true)