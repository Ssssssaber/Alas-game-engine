#pragma once

#include "Application.h"

extern Alas::Application* Alas::CreateApplication();

int main(int argv, char** argc)
{
    Alas::Logger::Init();
    auto app = Alas::CreateApplication();
    app->Run();
    delete app;

    return 0;
}