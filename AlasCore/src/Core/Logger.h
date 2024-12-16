#pragma once

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Alas {

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

#define ALAS_CORE_FATAL(...) Alas::Logger::GetCoreLogger()->critical(_VA_ARGS__)
#define ALAS_CORE_ERROR(...) Alas::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define ALAS_CORE_WARN(...) Alas::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define ALAS_CORE_INFO(...) Alas::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define ALAS_CORE_TRACE(...) Alas::Logger::GetCoreLogger()->trace(__VA_ARGS__)


#define ALAS_CLIENT_FATAL(...) Alas::Logger::GetClientLogger()->critical(__VA_ARGS__)
#define ALAS_CLIENT_ERROR(...) Alas::Logger::GetClientLogger()->error(__VA_ARGS__)
#define ALAS_CLIENT_WARN(...) Alas::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define ALAS_CLIENT_INFO(...) Alas::Logger::GetClientLogger()->info(__VA_ARGS__)
#define ALAS_CLIENT_TRACE(...) Alas::Logger::GetClientLogger()->trace(__VA_ARGS__)
