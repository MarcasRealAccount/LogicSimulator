#include "Utilities/Config/ConfigManager.h"
#include "Utilities/Config/ConfigFile.h"

std::unordered_map<std::string, ConfigFile*> ConfigManager::configFiles;

void ConfigManager::SaveConfigs() {
	for (auto& configFile : ConfigManager::configFiles) configFile.second->SaveConfig();
}

ConfigFile* ConfigManager::GetConfigFile(const std::string& id) {
	return ConfigManager::GetConfigFilePath("Configs/" + id);
}

ConfigFile* ConfigManager::GetConfigFilePath(const std::string& filePath) {
	auto itr = ConfigManager::configFiles.find(filePath);
	if (itr != ConfigManager::configFiles.end()) return itr->second;
	ConfigFile* file = new ConfigFile(filePath);
	file->ReadConfig();
	ConfigManager::configFiles.insert({ filePath, file });
	return file;
}

void ConfigManager::RemoveConfigFile(ConfigFile* configFile) {
	auto itr = ConfigManager::configFiles.find(configFile->GetKey());
	if (itr != ConfigManager::configFiles.end()) ConfigManager::configFiles.erase(itr);
}