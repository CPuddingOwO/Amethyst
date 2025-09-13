#pragma once

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace logging  {
    using lvl = spdlog::level::level_enum;
    static std::string default_pattern = "%^[%H:%M:%S] [%L/%n/%t] %v%$";
    static auto all = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/all.log", true);

    class Logger {
        std::shared_ptr<spdlog::logger> logger_;
        std::string name_;
    public:
        explicit Logger(const char* name, const lvl level = lvl::trace, const bool single_file = false) {
            std::vector<spdlog::sink_ptr> sinks;
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            sinks.push_back(all);
            if (single_file) {
                try {
                    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::format("logs/{}.log", name), true));
                } catch (const spdlog::spdlog_ex& ex) {
                    error("Logger -> {} : {}", name, ex.what());
                }
            }

            const auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
            this->name_ = name;
            this->SetInternalLogger(logger);
            this->SetLevel(level);
            this->SetPattern(default_pattern);
            this->debug("Initialized logger -> {}", this->name_);
        }
        ~Logger() {
            this->debug("Destroyed logger -> {}", this->name_);
        }
        void SetInternalLogger(std::shared_ptr<spdlog::logger> logger) {    this->logger_ = std::move(logger); }
        void SetLevel(const lvl level) const {                              this->logger_->set_level(level); }
        void SetPattern(const std::string& pattern) const {                 this->logger_->set_pattern(pattern); }
        void SetInternalSink(const std::shared_ptr<spdlog::sinks::sink> &sink) const {   this->logger_->sinks().push_back(std::move(sink)); }
        std::shared_ptr<spdlog::logger> GetInternalLogger() {               return logger_; }

        template <typename... Args>
        void error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            this->logger_->error( fmt, std::forward<Args>(args)... );
        }
        template <typename... Args>
        void info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            this->logger_->info( fmt, std::forward<Args>(args)... );
        }
        template <typename... Args>
        void debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            this->logger_->debug( fmt, std::forward<Args>(args)... );
        }
        template <typename... Args>
        void warn(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            this->logger_->warn( fmt, std::forward<Args>(args)... );
        }
        template <typename... Args>
        void critical(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            this->logger_->critical( fmt, std::forward<Args>(args)... );
        }
        template <typename... Args>
        void trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
            this->logger_->trace( fmt, std::forward<Args>(args)... );
        }
    };
    // static const std::shared_ptr<Logger> console;
    // static const std::shared_ptr<Logger> console;
    inline const auto console = std::make_shared<Logger>("Console");

    template <typename... Args>
    void error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        console->error( fmt, std::forward<Args>(args)... );
    }
    template <typename... Args>
    void info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        console->info( fmt, std::forward<Args>(args)... );
    }
    template <typename... Args>
    void debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        console->debug( fmt, std::forward<Args>(args)... );
    }
    template <typename... Args>
    void warn(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        console->warn( fmt, std::forward<Args>(args)... );
    }
    template <typename... Args>
    void critical(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        console->critical( fmt, std::forward<Args>(args)... );
    }
    template <typename... Args>
    void trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        console->trace( fmt, std::forward<Args>(args)... );
    }

}