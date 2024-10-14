/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#include "Logger.h"
#include <stdio.h>
#include <windows.h>
#include <chrono>
#include <time.h>
#include <cassert>

static auto DateTime = new char[26];

Logger::Logger()
{

}

Logger::~Logger()
{
    delete[] DateTime;
}

void Logger::LogMessage(const std::string& msg)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
    InternalLog("LOG", msg.c_str());
}

void Logger::LogError(const std::string& err)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
    InternalLog("ERROR", err.c_str());
}

void Logger::LogWarning(const std::string& wrn)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
    InternalLog("WARNING", wrn.c_str());
}

void Logger::LogFatal(const std::string& ftl)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
    InternalLog("FATAL", ftl.c_str());
    assert(false);
}

void Logger::InternalLog(const char* pre, const char* msg)
{
    const auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if (ctime_s(DateTime, 26, &timeNow) == 0)
    {
        // big brain hack to remove endl from dateTime and retain message formatting with []
        DateTime[24] = ']';

        printf("%s: [%s - %s\n", pre, DateTime, msg);
    }
}
