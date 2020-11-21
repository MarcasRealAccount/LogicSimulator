#pragma once

#include <string>
#include <unordered_map>
#include <vector>

template <typename T>
using EnumVector = std::vector<std::pair<T, std::string>>;

class ConfigFile;

struct ConfigSection {
public:
	ConfigSection(const std::string& key, ConfigSection* parent = nullptr);
	~ConfigSection();

	void CleanUp();

	const std::unordered_map<std::string, std::string>& GetConfigs() const;
	const std::unordered_map<std::string, ConfigSection*>& GetSections() const;

	void SetConfig(const std::string& key, const std::string& value);
	const std::string& GetConfig(const std::string& key, const std::string& def = "");
	ConfigSection* GetSection(const std::string& key) const;
	ConfigSection* GetOrCreateSection(const std::string& key);

	template <typename T>
	void SetConfigTyped(const std::string& key, T value);
	template <typename T>
	T GetConfigTyped(const std::string& key, T def);

	template <typename T>
	void SetConfigEnum(const std::string& key, T value, const EnumVector<T>& enumNames) {
		auto itr = enumNames.begin();
		while (itr != enumNames.end()) {
			if (itr->first == value) {
				SetConfig(key, itr->second);
				break;
			}
			itr++;
		}
	}

	template <typename T>
	T GetConfigEnum(const std::string& key, T def, const EnumVector<T>& enumNames) {
		auto itr = enumNames.begin();
		while (itr != enumNames.end()) {
			if (itr->first == def) break;
			itr++;
		}
		if (itr == enumNames.end()) return def;

		const std::string& configValue = GetConfig(key, itr->second);

		itr = enumNames.begin();
		while (itr != enumNames.end()) {
			if (itr->second == configValue) return itr->first;
			itr++;
		}
		return def;
	}

	const std::string& GetKey() const;
	std::string GetFullKey() const;

	friend ConfigFile;

protected:
	bool HasChanged() const;
	void SetChanged(bool changed);

private:
	std::string Save(bool first = true);
	std::string Save(bool& firstSection, bool first = true);

	void RemoveSection(ConfigSection* section);

private:
	std::string key;
	ConfigSection* parent;

	std::unordered_map<std::string, std::string> configs;
	std::unordered_map<std::string, ConfigSection*> sections;
	bool changed = false;
};