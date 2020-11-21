#pragma once

#include <string>
#include <unordered_map>

class ConfigFile;

class ConfigManager {
public:
	static void SaveConfigs();

	static ConfigFile* GetConfigFile(const std::string& id);
	static ConfigFile* GetConfigFilePath(const std::string& filePath);

	friend ConfigFile;

private:
	static void RemoveConfigFile(ConfigFile* configFile);

private:
	static std::unordered_map<std::string, ConfigFile*> configFiles;
};