#include "Logger.h"

namespace AGS {
    
    std::shared_ptr<spdlog::logger> Logger::_spdClientLogger;
    std::shared_ptr<spdlog::logger> Logger::_spdCoreLogger;
    
    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        _spdClientLogger = spdlog::stdout_color_mt("APP");
        _spdClientLogger->set_level(spdlog::level::trace);
        
        _spdCoreLogger = spdlog::stdout_color_mt("CORE");
        _spdCoreLogger->set_level(spdlog::level::trace);
    }
}
