#pragma once

#include <string>
#include <vector>
#include <unordered_set>

enum class Severity {
	INFO,
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
	void LogInfo(const char* format, ...);
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

private:
	static uint32_t GetSeverityMaxBufferCount(Severity severity);
	static const char* GetSeverityId(Severity severity);
	static const char* GetSeverityConsoleColor(Severity severity);

	static void Log(const std::string& name, Severity severity, const char* format, va_list args);
	static void Flush();

private:
	std::string name;

private:
	static std::unordered_set<Severity> DisabledSeverities;
	static std::unordered_set<std::string> DisabledLoggers;

	static std::vector<std::string> Buffer;

	static bool LogToFile;
	static bool LogToConsole;
	static const char* PreviousFile;
	static const char* CurrentFile;
};