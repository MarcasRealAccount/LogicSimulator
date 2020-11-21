#pragma once

#include "Utilities/Config/ConfigSection.h"

class ConfigFile : public ConfigSection {
public:
	ConfigFile(const std::string& id);
	~ConfigFile();

	void ReadConfig();
	void SaveConfig();
};