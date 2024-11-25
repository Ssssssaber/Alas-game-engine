#include "ImGui/ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "Core/Application.h"
#include "SDL.h"
#include "glad/glad.h"

ImGuiKey ImGui_ImplSDL2_KeyEventToImGuiKey(SDL_Keycode keycode, SDL_Scancode scancode);

namespace AGS {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
    {

    }

    ImGuiLayer::~ImGuiLayer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnAttach()
    {
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();


        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui_ImplSDL2_InitForOpenGL(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init();
    }

    void ImGuiLayer::OnDetach()
    {

    }

    void ImGuiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)SDL_GetTicks();
        io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
        _time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(std::bind(&ImGuiLayer::OnKeyPressedEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&ImGuiLayer::OnKeyReleasedEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<KeyTypedEvent>(std::bind(&ImGuiLayer::OnKeyTypedEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&ImGuiLayer::OnMouseButtonPressedEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&ImGuiLayer::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&ImGuiLayer::OnMouseScrolledEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&ImGuiLayer::OnWindowSizeChangedEvent, this, std::placeholders::_1));
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        int keyMods = event.GetKeyMod();
        io.AddKeyEvent(ImGuiMod_Ctrl, (keyMods & KMOD_CTRL) != 0);
        io.AddKeyEvent(ImGuiMod_Shift, (keyMods & KMOD_SHIFT) != 0);
        io.AddKeyEvent(ImGuiMod_Alt, (keyMods & KMOD_ALT) != 0);
        io.AddKeyEvent(ImGuiMod_Super, (keyMods & KMOD_GUI) != 0);
        
        ImGuiKey key = ImGui_ImplSDL2_KeyEventToImGuiKey(event.GetKeyCode(), static_cast<SDL_Scancode>(event.GetScanCode()));
        io.AddKeyEvent(key, true);
        
        return false;
    }
    
    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();   
        
        int keyMods = event.GetKeyMod();
        io.AddKeyEvent(ImGuiMod_Ctrl, (keyMods & KMOD_CTRL) != 0);
        io.AddKeyEvent(ImGuiMod_Shift, (keyMods & KMOD_SHIFT) != 0);
        io.AddKeyEvent(ImGuiMod_Alt, (keyMods & KMOD_ALT) != 0);
        io.AddKeyEvent(ImGuiMod_Super, (keyMods & KMOD_GUI) != 0);
  
        ImGuiKey key = ImGui_ImplSDL2_KeyEventToImGuiKey(event.GetKeyCode(), static_cast<SDL_Scancode>(event.GetScanCode()));
        io.AddKeyEvent(key, false);

        return false;
    }
    
    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
    {
        AGS_CLIENT_TRACE("___IMGUI___{0}", event.ToString());
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharactersUTF8(event._text);

        return false;
    }
    
    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
    {
        int keyCode = event.GetKeyCode() - 1;
        AGS_ASSERT(keyCode >= 0, "IMGUI: wrong mouse code")
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(keyCode, true);

        return false;
    }
    
    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
    {
        int keyCode = event.GetKeyCode() - 1;
        AGS_ASSERT(keyCode >= 0, "IMGUI: wrong mouse code")
        ImGuiIO& io = ImGui::GetIO();        
        io.AddMouseButtonEvent(keyCode, false);

        return false;
    }
    
    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(event.GetX(), event.GetY());

        return false;
    }
    
    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

        return false;
    }
    
    bool ImGuiLayer::OnWindowSizeChangedEvent(WindowResizeEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());
        return false;
    }
}