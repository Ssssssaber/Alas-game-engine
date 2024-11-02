#include <Alas.h>

class Sandbox : public AGS::Application
{
public:
    Sandbox()
    {

    }

    ~Sandbox()
    {

    }
};


AGS::Application* AGS::CreateApplication()
{
    return new Sandbox();
}
