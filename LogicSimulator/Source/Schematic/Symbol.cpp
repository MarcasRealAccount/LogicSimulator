#include "Schematic/Symbol.h"

#include <filesystem>
#include <string_view>

uint64_t NumberOfSplits(const std::string_view& str, uint64_t offset, char delimiter) {
	uint64_t i = 0;
	while (offset < str.length()) {
		uint64_t start = str.find_first_not_of(delimiter, offset);
		uint64_t end = str.find_first_of(delimiter, start);
		i++;
		if (end >= str.length())
			break;
		offset = end + 1;
	}
	return i;
}

uint64_t SplitString(const std::string_view& str, uint64_t offset, char delimiter, std::string_view* arr, uint64_t count) {
	uint64_t i = 0;
	for (; i < count; i++) {
		uint64_t start = str.find_first_not_of(delimiter, offset);
		uint64_t end = str.find_first_of(delimiter, start);
		arr[i] = str.substr(start, end - start);
		offset = end + 1;
	}
	if (i < count && offset < str.length()) {
		arr[i] = str.substr(str.find_first_not_of(delimiter, offset));
		i++;
	}
	return i;
}

namespace SymbolLoader {
	bool ParseLine(Symbol& symbol, std::string_view line) {
		if (line._Starts_with("name=")) {
			symbol.name = line.substr(5);
		} else if (line._Starts_with("inputs=[")) {
			std::string_view inputs = line.substr(8, line.find_first_of(']') - 8);
			uint64_t numInputs = NumberOfSplits(inputs, 0, ',');
			std::string_view* splittedStrings = new std::string_view[numInputs];

			for (uint64_t i = 0; i < SplitString(inputs, 0, ',', splittedStrings, numInputs); i++) {
				symbol.inputs.push_back(std::string(splittedStrings[i]));
			}
			delete[] splittedStrings;
		} else if (line._Starts_with("outputs=[")) {
			std::string_view outputs = line.substr(9, line.find_first_of(']') - 9);
			uint64_t numOutputs = NumberOfSplits(outputs, 0, ',');
			std::string_view* splittedStrings = new std::string_view[numOutputs];

			for (uint64_t i = 0; i < SplitString(outputs, 0, ',', splittedStrings, numOutputs); i++) {
				symbol.outputs.push_back(std::string(splittedStrings[i]));
			}
			delete[] splittedStrings;
		} else if (line._Starts_with("{")) {
			std::string_view truthTableEntry = line.substr(1, line.find_first_of('}') - 1);
			uint64_t numVariables = NumberOfSplits(truthTableEntry, 0, ',');
			std::string_view* splittedStrings = new std::string_view[numVariables];

			TruthTableEntry& entry = symbol.truthTable.CreateEntry();
			for (uint64_t i = 0; i < SplitString(truthTableEntry, 0, ',', splittedStrings, numVariables); i++) {
				std::string_view var[2];
				uint64_t c = SplitString(splittedStrings[i], 0, '=', var, 2);
				if (c < 2) continue;

				// Check if the var is an input
				bool found = false;
				for (auto& in : symbol.inputs) {
					if (in == var[0]) {
						entry.AddInputValue(std::string(var[0]), atoi(var[1].data()));
						found = true;
						break;
					}
				}
				if (!found) {
					for (auto& out : symbol.outputs) {
						if (out == var[0]) {
							entry.AddOutputValue(std::string(var[0]), atoi(var[1].data()));
							found = true;
							break;
						}
					}
				}
			}
			delete[] splittedStrings;
		}

		return true;
	}
}

void TruthTableEntry::AddInputValue(const std::string& id, bool value) {
	this->inputs.push_back({ id, value });
}

void TruthTableEntry::AddOutputValue(const std::string& id, bool value) {
	this->outputs.push_back({ id, value });
}

const std::vector<std::pair<std::string, bool>>& TruthTableEntry::GetInputs() const {
	return this->inputs;
}

const std::vector<std::pair<std::string, bool>>& TruthTableEntry::GetOutputs() const {
	return this->outputs;
}

TruthTableEntry& TruthTable::CreateEntry() {
	entries.push_back({});
	return entries[entries.size() - 1];
}

bool TruthTable::GetEntry(const TruthTableEntry*& entry, const std::vector<std::pair<std::string, bool>>& inputs) const {
	for (uint64_t i = 0; i < this->entries.size(); i++) {
		bool foundEntry = true;
		for (auto& input : inputs) {
			bool found = false;
			for (auto& in : this->entries[i].GetInputs()) {
				if (in.first == input.first && in.second == input.second) {
					found = true;
					break;
				}
			}
			if (!found) {
				foundEntry = false;
				break;
			}
		}
		if (foundEntry) {
			entry = &this->entries[i];
			return true;
		}
	}
	return false;
}

bool Symbol::GetEntry(const TruthTableEntry*& entry, const std::vector<std::pair<std::string, bool>>& inputs) const {
	return truthTable.GetEntry(entry, inputs);
}

bool Symbol::GetOutputs(const std::vector<std::pair<std::string, bool>>& inputs, std::vector<std::pair<std::string, bool>>& outputs) const {
	outputs.clear();
	const TruthTableEntry* entry;
	if (!GetEntry(entry, inputs)) {
		return false;
	}

	for (auto& out : entry->GetOutputs()) {
		outputs.push_back(out);
	}
	return true;
}

void Symbol::Render() {

}

bool LoadSymbol(Symbol& symbol, std::string symbolFilePath) {
	FILE* file = fopen(symbolFilePath.c_str(), "r");
	if (!file)
		return false;
	fseek(file, 0, SEEK_END);
	uint64_t length = ftell(file);
	std::string str = std::string(length, '\0');
	fseek(file, 0, SEEK_SET);
	fread(str.data(), sizeof(char), length, file);
	fclose(file);

	uint64_t offset = 0;
	uint64_t i;
	while ((i = str.find_first_of('\n', offset)) < str.length()) {
		std::string_view line = std::string_view(str).substr(offset, i - offset);
		if (!SymbolLoader::ParseLine(symbol, line)) {
			return false;
		}
		offset = i + 1;
	}

	if (offset < str.length()) {
		std::string_view line = std::string_view(str).substr(offset, i);
		if (!SymbolLoader::ParseLine(symbol, line)) {
			return false;
		}
	}
	return true;
}