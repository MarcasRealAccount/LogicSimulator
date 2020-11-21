local currentPath, verbose = ...

local globalApp = require("premake/app")

local glfw = globalApp.third_party_library("glfw", currentPath, verbose)
local glad = globalApp.third_party_library("glad", currentPath, verbose)
local glm = globalApp.third_party_library("glm", currentPath, verbose)

local app = globalApp.app("LogicSimulator", currentPath, verbose)
app.group = "Apps"

globalApp.addDependency(app, glfw, verbose)
globalApp.addDependency(app, glad, verbose)
globalApp.addDependency(app, glm, verbose)

globalApp.addState(app, { filter = "system:linux", premakeState = function()
	linkoptions { "-pthread -Wl, -rpath=\\$$ORIGIN" }
	links { "dl" }
end}, verbose)
globalApp.addState(app, { filter = "system:ios", premakeState = function()
	files {
		app.currentPath .. app.resourceDir .. "Info.plist",
		app.currentPath .. app.resourceDir
	}
end}, verbose)
globalApp.addState(app, { filter = { "system:macosx or ios", "files:**.cpp" }, premakeState = function()
	compileas "Objective-C++"
end}, verbose)
globalApp.addState(app, { filter = "configurations:Debug", premakeState = function()
	kind "ConsoleApp"
end}, verbose)
globalApp.addState(app, { filter = "configurations:Release", premakeState = function()
	kind "WindowedApp"
end}, verbose)

return app