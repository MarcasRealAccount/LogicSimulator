local currentPath, verbose = ...

local globalApp = require("../premake/app")

local app = globalApp.app("glfw", currentPath, verbose)
app.kind = "StaticLib"
app.includeDir = "include"
app.sourceDir = "src"

globalApp.addFile(app, "include/GLFW/glfw3.h", verbose)
globalApp.addFile(app, "include/GLFW/glfw3native.h", verbose)
globalApp.addFile(app, "src/glfw_config.h", verbose)
globalApp.addFile(app, "src/context.c", verbose)
globalApp.addFile(app, "src/init.c", verbose)
globalApp.addFile(app, "src/input.c", verbose)
globalApp.addFile(app, "src/monitor.c", verbose)
globalApp.addFile(app, "src/vulkan.c", verbose)
globalApp.addFile(app, "src/window.c", verbose)

globalApp.addState(app, { filter = "system:windows", premakeState = function()
	files {
		app.currentPath .. "src/win32_init.c",
		app.currentPath .. "src/win32_joystick.c",
		app.currentPath .. "src/win32_monitor.c",
		app.currentPath .. "src/win32_time.c",
		app.currentPath .. "src/win32_thread.c",
		app.currentPath .. "src/win32_window.c",
		app.currentPath .. "src/wgl_context.c",
		app.currentPath .. "src/egl_context.c",
		app.currentPath .. "src/osmesa_context.c"
	}
	
	defines {
		"_GLFW_WIN32"
	}
	
	links("opengl32.lib")
end}, verbose)
globalApp.addState(app, { filter = "system:linux", premakeState = function()
	files {
		app.currentPath .. "src/x11_init.c",
		app.currentPath .. "src/x11_monitor.c",
		app.currentPath .. "src/x11_window.c",
		app.currentPath .. "src/x11_unicode.c",
		app.currentPath .. "src/posix_time.c",
		app.currentPath .. "src/posix_thread.c",
		app.currentPath .. "src/glx_context.c",
		app.currentPath .. "src/egl_context.c",
		app.currentPath .. "src/osmesa_context.c",
		app.currentPath .. "src/linux_joystick.c"
	}
	
	defines {
		"_GLFW_X11"
	}
end}, verbose)

return app