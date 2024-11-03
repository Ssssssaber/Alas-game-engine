#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Logger.h"
namespace AGS
{

    Application::Application() {}

    Application::~Application() {}
    
    void Application::Run()
    {
        WindowResizeEvent e(1024, 768);
        if (e.isInCategory(EventCategoryApplication))
        {
            // AGS_CORE_INFO(e);
            AGS_CORE_INFO("e");
        }
        if (e.isInCategory(EventCategoryInput))
        {
            AGS_CORE_INFO("e");
        }
        
        while(1); 
    }
}