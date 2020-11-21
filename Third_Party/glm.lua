local currentPath, verbose = ...

local globalApp = require("../premake/app")

local app = globalApp.app("glm", currentPath, verbose)
app.kind = "StaticLib"
app.includeDir = ""
app.sourceDir = ""

globalApp.addFile(app, "glm/**.h", verbose)
globalApp.addFile(app, "glm/**.inl", verbose)
globalApp.addFile(app, "glm/**.hpp", verbose)
globalApp.addFile(app, "glm/**.cpp", verbose)

return app