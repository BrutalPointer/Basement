// ---------------- include/LogLevel.hpp ----------------
#pragma once

#include "Singleton.h"

#include <string>
#include <fstream>

enum class LogLevel { Debug, Info, Warning, Error };

class Logger {
public:
    Logger(LogLevel level, const std::string& filepath)
        : _logLevel(level), _writeLog(filepath, std::ios::app) {}

    void Log(LogLevel level, const std::string& message) {
        if (level >= _logLevel) {
            _writeLog << "[LOG] " << message << std::endl;
        }
    }

private:
    LogLevel _logLevel;
    std::ofstream _writeLog;
};