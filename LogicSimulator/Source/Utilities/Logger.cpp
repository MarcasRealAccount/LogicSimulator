#include "Utilities/Logger.h"

#include <ctime>
#include <stdio.h>
#include <stdarg.h>
#include <filesystem>

std::unordered_set<Severity> Logger::disabledSeverities;
std::unordered_set<std::string> Logger::disabledLoggers;

std::vector<std::string> Logger::buffer;

bool Logger::logToFile = true;
#ifdef _DEBUG
bool Logger::logToConsole = true;
#else
bool Logger::logToConsole = false;
#endif
const char* Logger::previousFile = "Logs/log-previous.txt";
const char* Logger::currentFile = "Logs/log-current.txt";

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

void Logger::LogTrace(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Logger::Log(this->name, Severity::TRACE, format, args);
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
	if (std::filesystem::exists(Logger::currentFile)) {
		if (std::filesystem::exists(Logger::previousFile)) std::filesystem::remove(Logger::previousFile);

		if (rename(Logger::currentFile, Logger::previousFile)) Logger("Logger").LogDebug("Failed to rename log file %s to %s", Logger::currentFile, Logger::previousFile);
	}
}

void Logger::DeInit() {
	Flush();
}

void Logger::EnableSeverity(Severity severity) {
	Logger::disabledSeverities.erase(severity);
}

void Logger::DisableSeverity(Severity severity) {
	Logger::disabledSeverities.insert(severity);
}

void Logger::EnableLogger(const std::string& name) {
	Logger::disabledLoggers.erase(name);
}

void Logger::DisableLogger(const std::string& name) {
	Logger::disabledLoggers.insert(name);
}

void Logger::Flush() {
	if (!Logger::logToFile) return;

	std::filesystem::path filepath{ Logger::currentFile };
	std::filesystem::create_directories(filepath.parent_path());

	FILE* file = fopen(Logger::currentFile, "a");
	if (file) {
		for (auto message : Logger::buffer) fwrite(message.c_str(), sizeof(char), message.length(), file);
		fclose(file);
		Logger::buffer.clear();
	} else {
		Logger::logToFile = false;
	}
}

void Logger::Log(const std::string& name, Severity severity, const char* format, va_list args) {
	if (!Logger::logToFile && !Logger::logToConsole) return;

	auto itr2 = Logger::disabledLoggers.find(name);
	if (itr2 != Logger::disabledLoggers.end()) return;

	auto itr = Logger::disabledSeverities.find(severity);
	if (itr != Logger::disabledSeverities.end()) return;

	uint32_t length = vsnprintf(nullptr, 0, format, args) + 1;
	std::string message(length, '\0');
	vsnprintf(message.data(), message.length(), format, args);

	std::vector<std::string> messages;

	uint32_t lastIndex = 0;
	for (uint32_t i = 0; i < message.length(); i++) {
		if (message[i] == '\n') {
			messages.push_back(message.substr(lastIndex, i - lastIndex));
			lastIndex = i + 1;
		} else if (i == message.length() - 1) {
			messages.push_back(message.substr(lastIndex));
		}
	}

	bool firstLine = true;
	uint64_t logMsgSpaces = 0;
	uint64_t consoleMsgSpaces = 0;
	for (std::string msg : messages) {
		std::string logMsg(logMsgSpaces, ' ');
		std::string consoleMsg(consoleMsgSpaces, ' ');

		if (firstLine) {
			constexpr uint32_t timeBufferSize = 16;
			std::time_t currentTime = std::time(nullptr);
			char timeBuffer[timeBufferSize];

			if (Logger::logToFile) logMsg += "[" + std::string(name) + "]";
			if (Logger::logToConsole) consoleMsg += "[" + std::string(name) + "]";

			if (std::strftime(timeBuffer, timeBufferSize, "[%H:%M:%S]", std::localtime(&currentTime))) {
				if (Logger::logToFile) logMsg += timeBuffer;
				if (Logger::logToConsole) consoleMsg += timeBuffer;
			}

			if (Logger::logToFile) {
				logMsg += " " + std::string(Logger::GetSeverityId(severity)) + ": ";
				logMsgSpaces = logMsg.length();
			}

			if (Logger::logToConsole) {
				std::string severityConsoleColor = std::string(Logger::GetSeverityConsoleColor(severity));
				static std::string defaultConsoleColor = "\033[0m";

				consoleMsg += " " + severityConsoleColor + std::string(Logger::GetSeverityId(severity)) + defaultConsoleColor + ": ";
				consoleMsgSpaces = consoleMsg.length() - severityConsoleColor.length() - defaultConsoleColor.length();
			}

			firstLine = false;
		}
		
		if (Logger::logToFile) {
			logMsg += msg + "\n";
			Logger::buffer.push_back(logMsg);
		}

		if (Logger::logToConsole) {
			consoleMsg += msg + "\n";
			printf("%s", consoleMsg.c_str());
		}
	}

	if (Logger::logToFile) {
		if (Logger::buffer.size() > Logger::GetSeverityMaxBufferCount(severity)) {
			Logger::Flush();
		}
	}
}

uint32_t Logger::GetSeverityMaxBufferCount(Severity severity) {
	switch (severity) {
	case Severity::DEBUG:
		return 50;
	case Severity::WARNING:
		return 10;
	case Severity::ERROR:
		return 0;
	case Severity::TRACE:
	default:
		return 100;
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
	case Severity::TRACE:
	default:
		return "Trace";
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
	case Severity::TRACE:
	default:
		return "\033[0;97m";
	}
}