#ifndef COMMON_LOGGER_HPP
#define COMMON_LOGGER_HPP

#include <cstdint>
#ifndef SPDLOG_ACTIVE_LEVEL
    #ifdef NDEBUG
        #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_WARN
    #else
        #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
    #endif
#endif

#include <format>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string_view>

namespace common {

enum class Level : std::uint8_t
{
    trace = SPDLOG_LEVEL_TRACE,
    debug = SPDLOG_LEVEL_DEBUG,
    info = SPDLOG_LEVEL_INFO,
    warn = SPDLOG_LEVEL_WARN,
    error = SPDLOG_LEVEL_ERROR,
    critical = SPDLOG_LEVEL_CRITICAL,
    off = SPDLOG_LEVEL_OFF
};

class Logger final
{
  public:
    static auto instance() -> Logger &
    {
        static Logger instance;
        return instance;
    }

    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    ~Logger() = default;
    auto operator=(const Logger &) -> Logger & = delete;
    auto operator=(Logger &&) -> Logger & = delete;

    void setLevel(Level level) { logger->set_level(static_cast<spdlog::level::level_enum>(level)); }

    void setPattern(std::string_view pattern) { logger->set_pattern(std::string(pattern)); }

    void flush() { logger->flush(); }

    template<typename... Args>
    void trace(std::format_string<Args...> fmt, Args &&...args)
    {
        logger->trace(std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void debug(std::format_string<Args...> fmt, Args &&...args)
    {
        logger->debug(std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void info(std::format_string<Args...> fmt, Args &&...args)
    {
        logger->info(std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void warn(std::format_string<Args...> fmt, Args &&...args)
    {
        logger->warn(std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void error(std::format_string<Args...> fmt, Args &&...args)
    {
        logger->error(std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void critical(std::format_string<Args...> fmt, Args &&...args)
    {
        logger->critical(std::format(fmt, std::forward<Args>(args)...));
    }

  private:
    Logger()
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        logger = std::make_shared<spdlog::logger>("vhdl_fmt", console_sink);
        logger->set_level(console_sink->level());
        logger->flush_on(spdlog::level::err);
    }

    std::shared_ptr<spdlog::logger> logger;
};

} // namespace common

#endif
