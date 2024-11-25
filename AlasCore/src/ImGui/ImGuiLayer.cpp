#include "ImGuiLayer.h"

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

        // io.keyma[ImGuiKey_Tab] = SDLK_TAB;
		// io.KeyMap[ImGuiKey_LeftArrow] = SDLK_LEFT;
		// io.KeyMap[ImGuiKey_RightArrow] = SDLK_LEFT;
		// io.KeyMap[ImGuiKey_UpArrow] = SDLK_UP;
		// io.KeyMap[ImGuiKey_DownArrow] = SDLK_DOWN;
		// io.KeyMap[ImGuiKey_PageUp] = SDLK_PAGEUP;
		// io.KeyMap[ImGuiKey_PageDown] = SDLK_PAGEDOWN;
		// io.KeyMap[ImGuiKey_Home] = SDLK_HOME;
		// io.KeyMap[ImGuiKey_End] = SDLK_END;
		// io.KeyMap[ImGuiKey_Insert] = SDLK_INSERT;
		// io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
		// io.KeyMap[ImGuiKey_Backspace] = SDLK_ESCAPE;
		// io.KeyMap[ImGuiKey_Space] = SDLK_SPACE;
		// io.KeyMap[ImGuiKey_Enter] = SDLK_KP_ENTER;
		// io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
		// io.KeyMap[ImGuiKey_A] = SDLK_a;
		// io.KeyMap[ImGuiKey_C] = SDLK_c;
		// io.KeyMap[ImGuiKey_V] = SDLK_v;
		// io.KeyMap[ImGuiKey_X] = SDLK_x;
		// io.KeyMap[ImGuiKey_Y] = SDLK_y;
		// io.KeyMap[ImGuiKey_Z] = SDLK_z;
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
        // dispatcher.Dispatch<KeyPressedEvent>(std::bind(&ImGuiLayer::OnKeyPressedEvent, this, std::placeholders::_1));
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        // ImGuiKey key = ImGui_ImplSDL2_KeyEventToImGuiKey(event.GetKeyCode(), static_cast<SDL_Scancode>(event.GetScanCode()));
        // io.AddKeyEvent(key, false);
    
        // io.SetKeyEventNativeData((ImGuiKey)event.GetKeyCode(), true);

        return false;
    }
    
    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();        
        // io.AddKeyEvent((ImGuiKey)event.GetKeyCode(), false);
        // ImGuiKey key = ImGui_ImplSDL2_KeyEventToImGuiKey(event.GetKeyCode(), static_cast<SDL_Scancode>(event.GetScanCode()));
        // io.AddKeyEvent(key, false);
    

        // io.KeyShift = io.KeysData[SDLK_LSHIFT].Down | io.KeysData[SDLK_RSHIFT].Down;
        // io.KeyCtrl = io.KeysData[SDLK_LCTRL].Down | io.KeysData[SDL_SCANCODE_RSHIFT].Down;
        // io.KeyAlt = io.KeysData[SDLK_LALT].Down | io.KeysData[SDLK_RALT].Down;

        return false;
    }
    
    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();        
        // int keycode = event.GetKeyCode();
        // if (keycode > 0 && keycode < 0x10000)
        // {
        //     io.AddInputCharacter((unsigned short)keycode);
        // }

        return false;
    }
    
    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        // io.AddMouseButtonEvent(event.GetKeyCode() - 1, true);
        // io.KeysData[0].Down = true;

        return false;
    }
    
    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        
        // io.AddMouseButtonEvent(event.GetKeyCode() - 1, false);
        // io.KeysData[0].Down = false;

        return false;
    }
    
    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        // io.AddMousePosEvent(event.GetX(), event.GetY());

        return false;
    }
    
    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
    {
        // ImGuiIO& io = ImGui::GetIO();
        // io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());

        return false;
    }
    
    bool ImGuiLayer::OnWindowSizeChangedEvent(WindowResizeEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        // io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
        // io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        // glViewport(0, 0, event.GetWidth(), event.GetHeight());
        return false;
    }
}