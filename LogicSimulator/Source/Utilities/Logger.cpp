#include "Utilities/Logger.h"

#include <ctime>
#include <stdio.h>
#include <stdarg.h>
#include <filesystem>

std::unordered_set<Severity> Logger::DisabledSeverities;
std::unordered_set<std::string> Logger::DisabledLoggers;

std::vector<std::string> Logger::Buffer;

bool Logger::LogToFile = true;
#ifdef _DEBUG
bool Logger::LogToConsole = true;
#else
bool Logger::LogToConsole = false;
#endif
const char* Logger::PreviousFile = "Logs/log-previous.txt";
const char* Logger::CurrentFile = "Logs/log-current.txt";

Logger::Logger(const std::string& name)
	: name(name) {}

void Logger::Enable() {
	Logger::EnableLogger(this->name);
}

void Logger::Disable() {
	Logger::DisableLogger(this->name);
}

void Logger::Log(Severity severity, const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(this->name, severity, format, args);
	va_end(args);
}

void Logger::LogInfo(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(this->name, Severity::INFO, format, args);
	va_end(args);
}

void Logger::LogDebug(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(this->name, Severity::DEBUG, format, args);
	va_end(args);
}

void Logger::LogWarning(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(this->name, Severity::WARNING, format, args);
	va_end(args);
}

void Logger::LogError(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(this->name, Severity::ERROR, format, args);
	va_end(args);
}

void Logger::Init() {
	if (std::filesystem::exists(Logger::CurrentFile)) {
		if (std::filesystem::exists(Logger::PreviousFile)) std::filesystem::remove(Logger::PreviousFile);

		if (rename(Logger::CurrentFile, Logger::PreviousFile)) Logger("Logger").LogDebug("Failed to rename log file %s to %s", Logger::CurrentFile, Logger::PreviousFile);
	}
}

void Logger::DeInit() {
	Flush();
}

void Logger::EnableSeverity(Severity severity) {
	Logger::DisabledSeverities.erase(severity);
}

void Logger::DisableSeverity(Severity severity) {
	Logger::DisabledSeverities.insert(severity);
}

void Logger::EnableLogger(const std::string& name) {
	Logger::DisabledLoggers.erase(name);
}

void Logger::DisableLogger(const std::string& name) {
	Logger::DisabledLoggers.insert(name);
}

uint32_t Logger::GetSeverityMaxBufferCount(Severity severity) {
	switch (severity) {
	case Severity::ERROR:
		return 0;
	case Severity::WARNING:
		return 10;
	case Severity::DEBUG:
		return 20;
	case Severity::INFO:
	default:
		return 30;
	}
}

const char* Logger::GetSeverityId(Severity severity) {
	switch (severity) {
	case Severity::DEBUG:
		return "Debug";
	case Severity::WARNING:
		return "Warning";
	case Severity::ERROR:
		return "Error";
	case Severity::INFO:
	default:
		return "Info";
	}
}

const char* Logger::GetSeverityConsoleColor(Severity severity) {
	switch (severity) {
	case Severity::DEBUG:
		return "\033[0;36m";
	case Severity::WARNING:
		return "\033[0;93m";
	case Severity::ERROR:
		return "\033[0;91m";
	case Severity::INFO:
	default:
		return "\033[0;97m";
	}
}

void Logger::Log(const std::string& name, Severity severity, const char* format, va_list args) {
	if (!Logger::LogToFile && !Logger::LogToConsole) return;

	auto itr2 = Logger::DisabledLoggers.find(name);
	if (itr2 != Logger::DisabledLoggers.end()) return;

	auto itr = Logger::DisabledSeverities.find(severity);
	if (itr != Logger::DisabledSeverities.end()) return;

	uint64_t length = vsnprintf(nullptr, 0, format, args) + 1ULL;
	std::string str(length, '\0');
	vsnprintf(str.data(), str.length(), format, args);

	std::vector<std::string_view> lines;

	uint64_t offset = 0;
	uint64_t index;
	while ((index = str.find_first_of('\n', offset)) < str.length()) {
		lines.push_back(std::string_view(str).substr(offset, index - offset));
		offset = index + 1;
	}
	if (offset < str.length()) lines.push_back(std::string_view(str).substr(offset, str.length() - 1 - offset));

	bool firstLine = true;
	uint64_t logMsgHeaderLength;
	uint64_t consoleMsgHeaderLength;
	for (auto& line : lines) {
		std::string logMsg;
		std::string consoleMsg;
		// Only add the log header if it's the first line.
		if (firstLine) {
			std::string color = std::string(Logger::GetSeverityConsoleColor(severity));

			std::string nameStr(name);
			if (Logger::LogToFile) logMsg = "[" + nameStr + "]";
			if (Logger::LogToConsole) consoleMsg = color + logMsg;

			constexpr uint32_t timeBufferSize = 16;
			std::time_t currentTime = std::time(nullptr);
			char timeBuffer[timeBufferSize];

			if (std::strftime(timeBuffer, timeBufferSize, "[%H:%M:%S]", std::localtime(&currentTime))) {
				if (Logger::LogToFile) logMsg += timeBuffer;
				if (Logger::LogToConsole) consoleMsg += timeBuffer;
			}

			if (Logger::LogToFile) logMsg += " " + std::string(Logger::GetSeverityId(severity)) + ": ";
			if (Logger::LogToConsole) consoleMsg += ": ";
			logMsgHeaderLength = logMsg.length();
			consoleMsgHeaderLength = consoleMsg.length() - color.length();
			firstLine = false;
		} else {
			if (Logger::LogToFile) logMsg = std::string(logMsgHeaderLength, ' ');
			if (Logger::LogToConsole) consoleMsg = std::string(consoleMsgHeaderLength, ' ') + std::string(Logger::GetSeverityConsoleColor(severity));
		}
		if (Logger::LogToFile) logMsg += std::string(line) + "\n";
		if (Logger::LogToConsole) consoleMsg += std::string(line) + "\033[0m\n";

		if (Logger::LogToFile) Logger::Buffer.push_back(logMsg);
		if (Logger::LogToConsole) printf("%s", consoleMsg.c_str());
	}

	if (Logger::LogToFile) {
		if (Logger::Buffer.size() > Logger::GetSeverityMaxBufferCount(severity)) {
			Logger::Flush();
		}
	}
}

void Logger::Flush() {
	if (!Logger::LogToFile) return;

	std::filesystem::path filepath{ Logger::CurrentFile };
	std::filesystem::create_directories(filepath.parent_path());

	FILE* file = fopen(Logger::CurrentFile, "a");
	if (file) {
		for (auto& str : Logger::Buffer) fwrite(str.c_str(), sizeof(char), str.length(), file);
		fclose(file);
		Logger::Buffer.clear();
	} else {
		Logger::LogToFile = false;
	}
}