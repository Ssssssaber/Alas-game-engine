#pragma once

#include "Application.h"

extern AGS::Application* AGS::CreateApplication();

int main(int argv, char** argc)
{
    auto app = AGS::CreateApplication();
    app->Run();
    delete app;
}