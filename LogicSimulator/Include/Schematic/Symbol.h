#pragma once

#include <vector>
#include <string>

struct TruthTableEntry {
public:
	void AddInputValue(const std::string& id, bool value);
	void AddOutputValue(const std::string& id, bool value);

	const std::vector<std::pair<std::string, bool>>& GetInputs() const;
	const std::vector<std::pair<std::string, bool>>& GetOutputs() const;

private:
	std::vector<std::pair<std::string, bool>> inputs;
	std::vector<std::pair<std::string, bool>> outputs;
};

struct TruthTable {
public:
	TruthTableEntry& CreateEntry();
	bool GetEntry(const TruthTableEntry*& entry, const std::vector<std::pair<std::string, bool>>& inputs) const;

private:
	std::vector<TruthTableEntry> entries;
};

struct Symbol {
public:
	bool GetEntry(const TruthTableEntry*& entry, const std::vector<std::pair<std::string, bool>>& inputs) const;
	bool GetOutputs(const std::vector<std::pair<std::string, bool>>& inputs, std::vector<std::pair<std::string, bool>>& outputs) const;

	void Render();

public:
	std::string name;
	
	std::vector<std::string> inputs;
	std::vector<std::string> outputs;

	TruthTable truthTable;
};

bool LoadSymbol(Symbol& symbol, std::string symbolFilePath);