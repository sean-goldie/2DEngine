#pragma once

#include <string>

/**
 * Stupid simple logging class - with pretty colors!
 */
class Logger
{
public:
	Logger();
	~Logger();

	static void LogMessage(const std::string& msg);
	static void LogError(const std::string& err);
	static void LogWarning(const std::string& wrn);
	static void LogFatal(const std::string& ftl);

private:
	static void InternalLog(const char* pre, const char* msg);
};