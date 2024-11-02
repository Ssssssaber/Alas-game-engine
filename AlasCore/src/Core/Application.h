#pragma once

namespace AGS
{
    class Application 
    {
    public:
        Application();

        virtual ~Application();
        
        void Run();
    };

    Application* CreateApplication();
} // namespace AGS

