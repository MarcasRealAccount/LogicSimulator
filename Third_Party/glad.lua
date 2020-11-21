local currentPath, verbose = ...

local globalApp = require("../premake/app")

local app = globalApp.app("glad", currentPath, verbose)
app.kind = "StaticLib"
app.includeDir = "include/"
app.sourceDir = "src/"

globalApp.addFile(app, "include/**.h", verbose)
globalApp.addFile(app, "src/**.c", verbose)

return app