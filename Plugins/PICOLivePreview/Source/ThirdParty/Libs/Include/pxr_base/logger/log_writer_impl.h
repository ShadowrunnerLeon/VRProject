#pragma once
#include "pxr_base/logger.h"

#ifdef PXR_WIN
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#elif defined(ANDROID)
#include "spdlog/sinks/android_sink.h"
#endif

#include <atomic>
namespace pxr_base
{
    class LogWriterImpl
    {
    public:
        static LogWriterImpl* Instance();
        void SetLogFilePrefix(const std::string& name);
        void Write(LogMessage& msg);
        void SetLogLevel(LogLevel level); 
        std::string GetPidStr() const;
    private:
        LogWriterImpl();
        bool InitSpdLogger();
        inline uint64_t GetPid();
    private:
        std::atomic<LogLevel> output_level_ = LogLevel::kLogLevelInfo;
        std::shared_ptr<spdlog::logger> logger_;
        std::atomic_bool inited_;
        std::mutex mutex_;
        std::string log_file_prefix_;
        std::string pid_;
    };
}