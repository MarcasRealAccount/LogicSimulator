#include "Utilities/Config/ConfigSection.h"

#include <sstream>

std::string ToParsableString(const std::string& str) {
	std::string output = str;

	bool space = false;
	for (uint64_t i = 0; i < output.length(); i++) {
		if (output[i] == '\\' && output[i + 1] == '\n') {
			i++;
			continue;
		}
		char c = output[i];

		switch (c) {
		case ' ':
			space = true;
			break;
		case '\\':
		case '\'':
		case '\"':
		case ';':
		case '#':
		case '=':
		case ':':
			output.insert(output.begin() + i, '\\');
			i++;
			break;
		case '\0':
			output[i] = '0';
			output.insert(output.begin() + i, '\\');
			i++;
			break;
		case '\a':
			output[i] = 'a';
			output.insert(output.begin() + i, '\\');
			i++;
			break;
		case '\b':
			output[i] = 'b';
			output.insert(output.begin() + i, '\\');
			i++;
			break;
		case '\t':
			output[i] = 't';
			output.insert(output.begin() + i, '\\');
			i++;
			break;
		case '\r':
			output[i] = 'r';
			output.insert(output.begin() + i, '\\');
			i++;
			break;
		case '\n':
			output[i] = 'n';
			output.insert(output.begin() + i, '\\');
			i++;
			break;
		}
	}

	if (space) {
		output = '"' + output + '"';
	}

	return output;
}

ConfigSection::ConfigSection(const std::string& key, ConfigSection* parent)
	: key(key), parent(parent) {}

ConfigSection::~ConfigSection() {
	if (this->parent) this->parent->RemoveSection(this);
}

void ConfigSection::CleanUp() {
	for (auto& section : this->sections) delete section.second;
	this->sections.clear();
	this->configs.clear();
}

const std::unordered_map<std::string, std::string>& ConfigSection::GetConfigs() const {
	return this->configs;
}

const std::unordered_map<std::string, ConfigSection*>& ConfigSection::GetSections() const {
	return this->sections;
}

void ConfigSection::SetConfig(const std::string& key, const std::string& value) {
	this->configs.insert_or_assign(key, value);
	this->changed = true;
}

const std::string& ConfigSection::GetConfig(const std::string& key, const std::string& def) {
	uint64_t keyPeriod = key.find_first_of('.');
	if (keyPeriod < key.length()) {
		std::string thisKey = key.substr(0, keyPeriod);
		ConfigSection* sec = GetOrCreateSection(thisKey);
		if (sec) return sec->GetConfig(key.substr(keyPeriod + 1), def);
		return def;
	}
	auto itr = this->configs.find(key);
	if (itr != this->configs.end()) return itr->second;
	SetConfig(key, def);
	return def;
}

ConfigSection* ConfigSection::GetSection(const std::string& key) const {
	uint64_t keyPeriod = key.find_first_of('.');
	if (keyPeriod < key.length()) {
		std::string thisKey = key.substr(0, keyPeriod);
		ConfigSection* sec = GetSection(thisKey);
		if (sec) return sec->GetSection(key.substr(keyPeriod + 1));
		return nullptr;
	}
	auto itr = this->sections.find(key);
	if (itr != this->sections.end()) return itr->second;
	return nullptr;
}

ConfigSection* ConfigSection::GetOrCreateSection(const std::string& key) {
	uint64_t keyPeriod = key.find_first_of('.');
	if (keyPeriod < key.length()) {
		std::string thisKey = key.substr(0, keyPeriod);
		ConfigSection* sec = GetOrCreateSection(thisKey);
		if (sec) return sec->GetOrCreateSection(key.substr(keyPeriod + 1));
		return nullptr;
	}
	auto itr = this->sections.find(key);
	if (itr != this->sections.end()) return itr->second;
	ConfigSection* section = new ConfigSection(key, this);
	this->sections.insert({ key, section });
	return section;
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, int8_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, int16_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, int32_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, int64_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, uint8_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, uint16_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, uint32_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, uint64_t value) {
	SetConfig(key, std::to_string(value));
}

template <>
void ConfigSection::SetConfigTyped(const std::string& key, bool value) {
	SetConfig(key, value ? "true" : "false");
}

template <>
int8_t ConfigSection::GetConfigTyped(const std::string& key, int8_t def) {
	int8_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
int16_t ConfigSection::GetConfigTyped(const std::string& key, int16_t def) {
	int16_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
int32_t ConfigSection::GetConfigTyped(const std::string& key, int32_t def) {
	int32_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
int64_t ConfigSection::GetConfigTyped(const std::string& key, int64_t def) {
	int64_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
uint8_t ConfigSection::GetConfigTyped(const std::string& key, uint8_t def) {
	uint8_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
uint16_t ConfigSection::GetConfigTyped(const std::string& key, uint16_t def) {
	uint16_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
uint32_t ConfigSection::GetConfigTyped(const std::string& key, uint32_t def) {
	uint32_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
uint64_t ConfigSection::GetConfigTyped(const std::string& key, uint64_t def) {
	uint64_t val;
	std::istringstream(GetConfig(key, std::to_string(def))) >> val;
	return val;
}

template <>
bool ConfigSection::GetConfigTyped(const std::string& key, bool def) {
	return GetConfig(key, def ? "true" : "false") == "true";
}

const std::string& ConfigSection::GetKey() const {
	return this->key;
}

std::string ConfigSection::GetFullKey() const {
	std::string key = this->key;
	if (this->parent) key = this->parent->GetFullKey() + "." + key;
	return key;
}

bool ConfigSection::HasChanged() const {
	if (this->changed) return true;
	for (auto& section : this->sections) if (section.second->HasChanged()) return true;
	return false;
}

void ConfigSection::SetChanged(bool changed) {
	this->changed = changed;
	for (auto& section : this->sections) section.second->SetChanged(changed);
}

std::string ConfigSection::Save(bool first) {
	bool firstSection = true;
	return Save(firstSection, first);
}

std::string ConfigSection::Save(bool& firstSection, bool first) {
	std::string output;
	if (this->configs.size() > 0) {
		if (!first) {
			std::string fullKey = GetFullKey();
			fullKey = fullKey.substr(fullKey.find_first_of('.') + 1);
			if (!firstSection) output += "\n";
			output += "[" + ToParsableString(fullKey) + "]\n";
			firstSection = false;
		}

		for (auto& config : this->configs) {
			output += ToParsableString(config.first) + " = " + ToParsableString(config.second) + "\n";
		}
	}

	for (auto& section : this->sections) {
		output += section.second->Save(firstSection, false);
	}
	return output;
}

void ConfigSection::RemoveSection(ConfigSection* section) {
	auto itr = this->sections.find(section->GetKey());
	if (itr != this->sections.end()) this->sections.erase(itr);
}