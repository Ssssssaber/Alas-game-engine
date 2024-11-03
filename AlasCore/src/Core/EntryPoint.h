#pragma once

#include "Application.h"

extern AGS::Application* AGS::CreateApplication();

int main(int argv, char** argc)
{
    auto app = AGS::CreateApplication();
    AGS::Logger::Init();
    AGS_CLIENT_INFO("KEKE IS YOU");
    AGS_CORE_WARN("NOT BABA IS YOU");
    app->Run();
    delete app;
}