local globalApp = {}
if not apps then
	apps = {}
end

function globalApp.third_party_app(name, currentPath, verbose)
	if apps[name] then
		return apps[name]
	end
	
	local module_path = "Third_Party/" .. name .. "/premakeApp.lua"
	local app = assert(loadfile(module_path))(currentPath .. "Third_Party/" .. name .. "/", verbose)
	app.warnings = "Off"
	apps[app.name] = app
	
	return app
end

function globalApp.third_party_library(name, currentPath, verbose)
	if apps[name] then
		return apps[name]
	end
	
	local module_path = "Third_Party/" .. name .. ".lua"
	local app = assert(loadfile(module_path))(currentPath .. "Third_Party/" .. name .. "/", verbose)
	app.warnings = "Off"
	apps[app.name] = app
	
	return app
end

function globalApp.local_app(verbose)
	if apps[name] then
		return apps[name]
	end
	
	local app = assert(loadfile("premakeApp.lua"))("", verbose)
	app.group = "Apps"
	apps[app.name] = app
	
	return app
end

function globalApp.app(name, currentPath, verbose)
	if apps[name] then
		return apps[name]
	end
	
	local app = {}
	
	app.cppDialect = "C++17"
	app.rtti = "Off"
	app.exceptionHandling = "On"
	app.flags = { "MultiProcessorCompile" }
	app.name = name
	app.currentPath = currentPath
	app.dependencies = {}
	app.group = "Libs"
	app.location = "Build/%{_ACTION}"
	app.objectDir = "Output/" .. name .. "/Obj/"
	app.outputDir = "Output/" .. name .. "/Bin/"
	app.libraryDir = "Output/" .. name .. "/Lib/"
	app.includeDir = name .. "/Include/"
	app.sourceDir = name .. "/Source/"
	app.resourceDir = name .. "/Assets/"
	app.debugDir = name .. "/Run/"
	app.warnings = "Default"
	
	app.states = {}
	
	apps[name] = app
	if verbose then
		print("Created app " .. name)
	end
	return app
end

function globalApp.addFlag(app, flag, verbose)
	table.insert(app.flags, flag)
	if verbose then
		print("Added flag " .. flag .. " to "  .. app.name)
	end
end

function globalApp.getLocalFilePath(app, file)
	return app.currentPath .. file
end

function globalApp.addFile(app, file, verbose)
	if not app.files then
		app.files = {}
	end

	table.insert(app.files, file)
	if verbose then
		print("Added file " .. file .. " to "  .. app.name)
	end
end

function globalApp.addDependency(app, dependency, verbose)
	app.dependencies[dependency.name] = dependency
	if verbose then
		print("Added dependency " .. dependency.name .. " to " .. app.name)
	end
end

function globalApp.addState(app, state, verbose)
	table.insert(app.states, state)
	if verbose then
		if type(state.filter) == "table" then
			local str = "Added state { "
			for i, filter in pairs(state.filter) do
				str = str .. '"' .. filter .. '"'
				if i < #state.filter then
					str = str .. ", "
				end
			end
			print(str .. " } to " .. app.name)
		else
			print("Added state " .. '"' .. state.filter .. '"' .. " to " .. app.name)
		end
	end
end

local function getAllIncludeDirectories(app, includeDirs)
	table.insert(includeDirs, app.currentPath .. app.includeDir)
	for name, dep in pairs(app.dependencies) do
		getAllIncludeDirectories(dep, includeDirs)
	end
end

local function premakeApp(app, verbose)
	if app.premaked then
		return
	end

	local deps = {}
	local sysincludedirectories = {}
	for name, dep in pairs(app.dependencies) do
		table.insert(deps, name)
		premakeApp(dep)
		getAllIncludeDirectories(dep, sysincludedirectories)
	end
	
	if verbose then
		print("Premake function called on app " .. app.name)
	end
	group(app.group)
	cppdialect(app.cppDialect)
	exceptionhandling(app.exceptionHandling)
	flags(app.flags)
	rtti(app.rtti)
	project(app.name)
	debugdir(app.debugDir)
	links(deps)
	location(app.currentPath .. app.location)
	xcodebuildresources(app.currentPath .. app.resourceDir)
	warnings(app.warnings)
	objdir(app.objectDir)
	includedirs(app.currentPath .. app.includeDir)
	sysincludedirs(sysincludedirectories)
	if app.files then
		local Files = {}
		for i, file in pairs(app.files) do
			table.insert(Files, app.currentPath .. file)
		end
		files(Files)
	else
		files({
			app.currentPath .. app.includeDir .. "**.h",
			app.currentPath .. app.includeDir .. "**.hpp",
			app.currentPath .. app.sourceDir .. "**.c",
			app.currentPath .. app.sourceDir .. "**.cpp"
		})
	end
	if app.kind then
		kind(app.kind)
	end
	
	for i, state in pairs(app.states) do
		filter(state.filter)
		state.premakeState()
	end
	
	if project().kind == "StaticLib" or project().kind == "SharedLib" then
		targetdir(app.currentPath .. app.libraryDir)
	else
		targetdir(app.currentPath .. app.outputDir)
	end
	
	filter("configurations:Debug")
		optimize("Off")
		symbols("On")
		defines({ "_DEBUG", "_CRT_SECURE_NO_WARNINGS" })
	
	filter("configurations:Release")
		optimize("Full")
		symbols("Off")
		defines({ "_RELEASE", "NDEBUG", "_CRT_SECURE_NO_WARNINGS" })
	
	filter("system:windows")
		toolset("msc")
		defines({ "NOMINMAX" })
	
	filter("system:not windows")
		toolset("gcc")
	
	filter("system:linux")
		debugenvs({ "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../%{OUTDIR}" })
	
	filter({})
	
	app.premaked = true
end

function globalApp.premakeWorkspace(WorkspaceName, Platforms, Configurations, startApp, verbose)
	if verbose then
		print("Premake workspace called with " .. startApp.name .. " as startup")
	end
	workspace(WorkspaceName)
	platforms(Platforms)
	configurations(Configurations)
	for name, app in pairs(apps) do
		premakeApp(app)
	end
	workspace(WorkspaceName)
	startproject(startApp.name)
end

return globalApp