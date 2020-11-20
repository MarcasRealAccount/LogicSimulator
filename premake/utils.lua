utils = {}

function utils.get_platforms()
	if _ACTION == "android-studio" then
		return { "armeabi-v7a", "arm64-v8a", "x86", "x64_64" }
	end
	
	if os.ishost("windows") then
		return { "x64" }
	end
	
	local arch = os.outputof("uname -m")
	return { arch }
end

return utils