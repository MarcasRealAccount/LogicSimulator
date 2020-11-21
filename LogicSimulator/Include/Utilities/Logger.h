#pragma once

#include <string>
#include <vector>
#include <unordered_set>

enum class Severity {
	TRACE,
	DEBUG,
	WARNING,
	ERROR
};

class Logger {
public:
	Logger(const std::string& name);

	void Enable();
	void Disable();

	void Log(Severity severity, const char* format, ...);
	void LogTrace(const char* format, ...);
	void LogDebug(const char* format, ...);
	void LogWarning(const char* format, ...);
	void LogError(const char* format, ...);

public:
	static void Init();
	static void DeInit();

	static void EnableSeverity(Severity severity);
	static void DisableSeverity(Severity severity);

	static void EnableLogger(const std::string& name);
	static void DisableLogger(const std::string& name);

	static void Flush();

private:
	static void Log(const std::string& name, Severity severity, const char* format, va_list args);

	static uint32_t GetSeverityMaxBufferCount(Severity severity);
	static const char* GetSeverityId(Severity severity);
	static const char* GetSeverityConsoleColor(Severity severity);

private:
	std::string name;

private:
	static std::unordered_set<Severity> disabledSeverities;
	static std::unordered_set<std::string> disabledLoggers;

	static std::vector<std::string> buffer;

	static bool logToFile;
	static bool logToConsole;
	static const char* previousFile;
	static const char* currentFile;
};