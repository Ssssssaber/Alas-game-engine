#include <Alas.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#include <gtc/type_ptr.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_internal.h>

#include "CustomScripts/Triangle.h"
class ExampleLayer : public Alas::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        _editorWindow = &Alas::Application::Get().GetWindow(); 
        _editorWindow->SetVSync(true);

        float height = _editorWindow->GetHeight();
        float width = _editorWindow->GetWidth();
        
        _camera.reset(new Alas::OrthCamera(-1.6f, 1.6f, -0.9f, 0.9f));
        _scene.reset(new Alas::Scene());

        _textureShader = Alas::Shader::Create("Assets/Shaders/TextureShader.shader");
        _textureShader->Bind();

        _baseTexture.reset(new Alas::Texture2D("Assets/Textures/goool.png"));
        _baseTexture->Bind();
        
        _mainGOTexture.reset(new Alas::Texture2D("Assets/Textures/cross.png"));
        _mainGOTexture->Bind();

        _triangle = _scene->CreateEntity("Main triangle");
        auto& script = _triangle.AddComponent<Alas::NativeScriptComponent>();
        script.Bind<Triangle>();

        auto& sprite = _triangle.AddComponent<Alas::SpriteComponent>(_mainGOTexture, _textureShader);
        sprite.Color = glm::vec3(0.5f);

        _triangle.GetComponent<Alas::TransformComponent>().Scale = glm::vec3(2.0f, 2.0f, 2.0f);

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Alas::Entity ent = _scene->CreateEntity("Quad");
                auto& mesh = ent.AddComponent<Alas::SpriteComponent>(_baseTexture, _textureShader);

                int a = i + 1, b = j + 1;
                mesh.Color = glm::normalize(glm::vec4(a, b, glm::abs(a - b), a + b));
                auto& transform = ent.GetComponent<Alas::TransformComponent>();
                transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
                transform.Position = glm::vec3(i * 0.3f, j * 0.3f, 0.0f);
            }
        }
        
    }

    void OnUpdate() override
    {
        Alas::Window::SetCurrentWindow(*_editorWindow);
        Alas::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Alas::RenderCommand::Clear();
        
        float deltaTime = Alas::Time::getDeltaTime();
        _timeElapsed += deltaTime;
        _framesElapsed += 1;

        if (_timeElapsed > 1.0)
        {
            _frameRate = _framesElapsed / _timeElapsed;
            _framesElapsed = 0;
            _timeElapsed = 0;
        }

        if (Alas::Input::IsKeyPressed(ALAS_KEY_W))
        {
            _cameraPos.y += _cameraSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_S))
        {
            _cameraPos.y -= _cameraSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_A))
        {
            _cameraPos.x -= _cameraSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_D))
        {
            _cameraPos.x += _cameraSpeed * deltaTime;
        }

        if (Alas::Input::IsKeyPressed(ALAS_KEY_Q))
        {
            _cameraRotation -= _cameraRotationSpeed * deltaTime;
        }
        if (Alas::Input::IsKeyPressed(ALAS_KEY_E))
        {
            _cameraRotation += _cameraRotationSpeed * deltaTime;
        }

        _camera->SetPosition(_cameraPos);
        _camera->SetRotation(_cameraRotation);

        Alas::Renderer::BeginScene(_camera);

        _scene->SceneUpdate();
        
        Alas::Renderer::EndScene();
    }

    void OnCreateObjectButton()
    {
        float delta = Alas::Time::getDeltaTime();
        float time = Alas::Time::GetTimeInSeconds();

        Alas::Entity ent = _scene->CreateEntity("Quad");
        auto& mesh = ent.AddComponent<Alas::SpriteComponent>(_baseTexture, _textureShader);

        mesh.Color = glm::vec3(abs(glm::sin(time)));
        auto& transform = ent.GetComponent<Alas::TransformComponent>();
        transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
        transform.Position = glm::vec3(delta * 20 + time / 250, delta * 20 - time / 250, 0.0f);
        transform.Rotation = glm::vec3(0.0f, 0.0f, time * 10);

        ALAS_CLIENT_INFO("{0} {1} {2}", delta, time, sin(time));
    }

    void OnImGuiRender()
    {
        bool open = true;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        window_flags |=  ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        ImGui::Begin("DockSpace Demo", &open, window_flags);
        ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::End();

        ImGui::Begin("Scene");
        
        if (ImGui::Button("Start game"))
        {
            Alas::Application::Get().StartGameLoop(_scene);
        }

        ImGui::InputFloat("FPS", &_frameRate);
        if (ImGui::Button("Create quad", ImVec2(200, 50)))
        {
            OnCreateObjectButton();
        }

        ImGui::SeparatorText("Game objects");

        auto objects = _scene->GetEntityMap();
        std::map<Alas::UID, Alas::Entity>::iterator it;      
        for (it = objects.begin(); it != objects.end(); it++)
        {
            Alas::Entity ent = it->second;
            std::string str = "ID: " + std::to_string((ent.GetUID())) + " " + ent.GetComponent<Alas::TagComponent>().Tag;
            if (ImGui::TreeNode(str.c_str()))
            {
                auto& transform = ent.GetComponent<Alas::TransformComponent>();
                ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.01f);
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation));
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.01f);
                ImGui::ColorEdit3("Color", glm::value_ptr(ent.GetComponent<Alas::SpriteComponent>().Color));
                ImGui::TreePop();
            }
        }

        ImGui::End();
    }

    void OnEvent(Alas::Event& event) override
    {
    
    }

    private:
        float _timeElapsed = 0;
        float _framesElapsed = 0;
        float _frameRate = 0;

        Alas::Window* _editorWindow;

        Alas::Shared<Alas::Scene> _scene;

        Alas::Entity _triangle;
        
        Alas::Shared<Alas::Shader> _textureShader;

        Alas::Shared<Alas::Texture2D> _baseTexture;
        Alas::Shared<Alas::Texture2D> _mainGOTexture;

        Alas::Shared<Alas::OrthCamera> _camera;
        glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
        float _cameraRotation = 0;

        float _cameraSpeed = 2.0f;
        float _cameraRotationSpeed = 15.0f;
};

class Sandbox : public Alas::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};


Alas::Application* Alas::CreateApplication()
{
    return new Sandbox();
}
