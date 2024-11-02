#pragma once

#include <memory>

#include "../../../vendor/spdlog/include/spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


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
