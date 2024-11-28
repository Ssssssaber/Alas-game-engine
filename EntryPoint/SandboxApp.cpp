#include <Alas.h>

class ExampleLayer : public AGS::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        
    }

    void OnUpdate() override
    {
        // AGS_CLIENT_INFO("ExampleLayer::Update");
    }

    void OnEvent(AGS::Event& event) override
    {
        // AGS_CLIENT_TRACE("{0}", event.ToString());
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
