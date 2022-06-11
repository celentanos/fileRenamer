#include "logger.h"

#include <QDebug>

#include <iostream>

LoggerCout::LoggerCout(const std::string& category)
{
    if (!category.empty())
        m_category = "[" + category + "]";
}

void LoggerCout::logTrace(const std::string& entry)
{
    std::cout << (this->currentTime() + " " + m_category + " [TRC] " + entry) << std::endl;
}

void LoggerCout::logDebug(const std::string& entry)
{
    std::cout << (this->currentTime() + " " + m_category + " [DBG] " + entry) << std::endl;
}

void LoggerCout::logInfo(const std::string& entry)
{
    std::cout << (this->currentTime() + " " + m_category + " [INF] " + entry) << std::endl;
}

void LoggerCout::logWarn(const std::string& entry)
{
    std::cerr << (this->currentTime() + " " + m_category + " [WRN] " + entry) << std::endl;
}

void LoggerCout::logError(const std::string& entry)
{
    std::cerr << (this->currentTime() + " " + m_category + " [ERR] " + entry) << std::endl;
}

void LoggerCout::logFatal(const std::string& entry)
{
    std::cerr << (this->currentTime() + " " + m_category + " [FAT] " + entry) << std::endl;
}
