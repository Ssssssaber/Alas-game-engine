#pragma once

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace AGS {

    class Logger
    {

    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> GetClientLogger() {return _spdClientLogger; }
        static std::shared_ptr<spdlog::logger> GetCoreLogger() {return _spdCoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> _spdClientLogger;
        static std::shared_ptr<spdlog::logger> _spdCoreLogger;
    };

}

#define AGS_CORE_FATAL(...) ::AGS::Logger::GetCoreLogger()->fatal(__VA_ARGS__)
#define AGS_CORE_ERROR(...) ::AGS::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define AGS_CORE_WARN(...) ::AGS::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define AGS_CORE_INFO(...) ::AGS::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define AGS_CORE_TRACE(...) ::AGS::Logger::GetCoreLogger()->error(__VA_ARGS__)


#define AGS_CLIENT_FATAL(...) ::AGS::Logger::GetClientLogger()->fatal(__VA_ARGS__)
#define AGS_CLIENT_ERROR(...) ::AGS::Logger::GetClientLogger()->error(__VA_ARGS__)
#define AGS_CLIENT_WARN(...) ::AGS::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define AGS_CLIENT_INFO(...) ::AGS::Logger::GetClientLogger()->info(__VA_ARGS__)
#define AGS_CLIENT_TRACE(...) ::AGS::Logger::GetClientLogger()->error(__VA_ARGS__)
