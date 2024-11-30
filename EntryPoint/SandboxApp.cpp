#include <Alas.h>

class ExampleLayer : public AGS::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        
    }

    void OnUpdate() override
    {           
        if (AGS::Input::IsMouseButtonPressed(AGS_MOUSE_BUTTON_LEFT))
        {
            AGS_CLIENT_INFO("ExampleLayer:: LEFT mouse button key pressed");    
        }

        if (AGS::Input::IsKeyPressed(AGS_KEY_TAB))
        {
            AGS_CLIENT_INFO("ExampleLayer:: TAB key pressed");    
        }
        
        
    }

    void OnEvent(AGS::Event& event) override
    {
        if (event.GetEventType() == AGS::EventType::MouseMoved)
        {
            AGS_CLIENT_INFO("{0} {1}", AGS::Input::GetMousePosition().first, AGS::Input::GetMousePosition().second);   
        }
        
    }
};

class Sandbox : public AGS::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        // PushOverlay(new AGS::ImGuiLayer());
    }

    ~Sandbox()
    {

    }
};


AGS::Application* AGS::CreateApplication()
{
    return new Sandbox();
}
