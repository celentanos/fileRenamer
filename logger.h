#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>

#include <memory>
#include <string>

#define LOG_ERR_NOT_IMPL \
    m_logger->logError("not implemented | " + std::string(__FUNCTION__) + " | " + std::to_string(__LINE__));

#define LOG_WRN_NOT_IMPL \
    m_logger->logWarn("not implemented | " + std::string(__FUNCTION__) + " | " + std::to_string(__LINE__));

#define LOG_FUNC \
    std::string(__FUNCTION__)

#define LOG_FUNC_LINE \
    std::string(__FUNCTION__) + " | " + std::to_string(__LINE__)

////////////////////////////////////////////////////////////////////////////////
/// class
////////////////////////////////////////////////////////////////////////////////

class ILogger {
public:
    virtual void logTrace(const std::string& entry) = 0;
    virtual void logDebug(const std::string& entry) = 0;
    virtual void logInfo(const std::string& entry) = 0;
    virtual void logWarn(const std::string& entry) = 0;
    virtual void logError(const std::string& entry) = 0;
    virtual void logFatal(const std::string& entry) = 0;
};

using LoggerPtr = std::shared_ptr<ILogger>;

////////////////////////////////////////////////////////////////////////////////
/// class
////////////////////////////////////////////////////////////////////////////////

class LoggerCout : public ILogger {

public:
    explicit LoggerCout(const std::string& category = std::string {});

    void logTrace(const std::string& entry) override;
    void logDebug(const std::string& entry) override;
    void logInfo(const std::string& entry) override;
    void logWarn(const std::string& entry) override;
    void logError(const std::string& entry) override;
    void logFatal(const std::string& entry) override;

private:
    std::string currentTime();
    std::string m_category;
};

////////////////////////////////////////////////////////////////////////////////
/// inline functions
////////////////////////////////////////////////////////////////////////////////

inline std::string LoggerCout::currentTime()
{
    return QDateTime::currentDateTime().toString(Qt::DateFormat::ISODateWithMs).toStdString();
}

#endif // LOGGER_H
