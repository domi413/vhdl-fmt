#ifndef COMMON_LOGGER_HPP
#define COMMON_LOGGER_HPP

#include <cstdint>
#include <memory>
#include <spdlog/logger.h>
#ifndef SPDLOG_ACTIVE_LEVEL
    #ifdef NDEBUG
        #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_WARN
    #else
        #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
    #endif
#endif

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
    auto operator=(const Logger &) -> Logger & = delete;
    Logger(Logger &&) = delete;
    auto operator=(Logger &&) -> Logger & = delete;
    ~Logger() = default;

    void setLevel(Level level)
    {
        logger_->set_level(static_cast<spdlog::level::level_enum>(level));
    }

    void setPattern(std::string_view pattern) { logger_->set_pattern(std::string(pattern)); }

    void flush() { logger_->flush(); }

    template<typename... Args>
    void trace([[maybe_unused]] std::format_string<Args...> fmt, [[maybe_unused]] Args &&...args)
    {
        SPDLOG_LOGGER_TRACE(logger_, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void debug([[maybe_unused]] std::format_string<Args...> fmt, [[maybe_unused]] Args &&...args)
    {
        SPDLOG_LOGGER_DEBUG(logger_, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void info([[maybe_unused]] std::format_string<Args...> fmt, [[maybe_unused]] Args &&...args)
    {
        SPDLOG_LOGGER_INFO(logger_, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void warn([[maybe_unused]] std::format_string<Args...> fmt, [[maybe_unused]] Args &&...args)
    {
        SPDLOG_LOGGER_WARN(logger_, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void error([[maybe_unused]] std::format_string<Args...> fmt, [[maybe_unused]] Args &&...args)
    {
        SPDLOG_LOGGER_ERROR(logger_, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void critical([[maybe_unused]] std::format_string<Args...> fmt, [[maybe_unused]] Args &&...args)
    {
        SPDLOG_LOGGER_CRITICAL(logger_, std::format(fmt, std::forward<Args>(args)...));
    }

  private:
    Logger() :
      logger_([] {
          auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

          console_sink->set_level(spdlog::level::warn);
          console_sink->set_level(spdlog::level::trace);
          console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

          auto logger = std::make_shared<spdlog::logger>("vhdl_fmt", console_sink);
          logger->set_level(console_sink->level());
          logger->flush_on(spdlog::level::err);

          return logger;
      }())
    {
    }

    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace common

#endif
