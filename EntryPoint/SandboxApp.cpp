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

#define BASE_BUTTON_WIDTH 125
#define BASE_BUTTON_HEIGHT 25

#define BASE_DRAG_STEP 0.01f
class ExampleLayer : public Alas::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        Alas::ScriptingEngine::LoadAndExecuteScript("main-go.lua");

        _editorWindow = &Alas::Application::Get().GetWindow(); 
        _editorWindow->SetVSync(true);

        float height = _editorWindow->GetHeight();
        float width = _editorWindow->GetWidth();
        
        _camera.reset(new Alas::OrthCamera(-1.6f, 1.6f, -0.9f, 0.9f));
        _scene.reset(new Alas::Scene());

        _textureShader = Alas::Shader::Create("Assets/Shaders/TextureShader.shader");
        _textureShader->Bind();

        _baseTexture = Alas::Texture::Create("Assets/Textures/wall.png");
        _baseTexture->Bind();
        
        // -------------- test scene load correctly

        _mainGOTexture = Alas::Texture::Create("Assets/Textures/keke.png");
        _mainGOTexture->Bind();

        _mainGo = _scene->CreateEntity("Main triangle");
        auto& script = _mainGo.AddComponent<Alas::NativeScriptComponent>();
        script.Bind<Triangle>();

        auto& sprite = _mainGo.AddComponent<Alas::SpriteComponent>(_mainGOTexture, _textureShader);
        sprite.Color = glm::vec4(1.0f);

        _mainGo.GetComponent<Alas::Transform>().Rotation = glm::vec3(0.0f, 0.0f, 30.0f);

        auto& body = _mainGo.AddComponent<Alas::RigidBody2D>(Alas::RigidBody2D::BodyType::Dynamic);
        body.GravityScale = 0;
        _mainGo.AddComponent<Alas::BoxCollider2D>();

        // -------------- test scene load correctly

        // for (int i = 0; i < 4; i++)
        // {
        //     for (int j = 0; j < 4; j++)
        //     {
        //         Alas::Entity ent = _scene->CreateEntity("Quad");
        //         auto& mesh = ent.AddComponent<Alas::SpriteComponent>(_baseTexture, _textureShader);

        //         int a = i + 1, b = j + 1;
        //         mesh.Color = glm::normalize(glm::vec4(a, b, glm::abs(a - b), a + b));
        //         auto& transform = ent.GetComponent<Alas::Transform>();
        //         transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
        //         transform.Position = glm::vec3(i * 0.3f, j * 0.3f, 0.0f);

        //         ent.AddComponent<Alas::BoxCollider2D>(glm::vec2(-0.5f), glm::vec2(1.0f));
        //         ent.AddComponent<Alas::RigidBody2D>(Alas::RigidBody2D::BodyType::Kinematic);
        //     }
        // }
    
    }

    void OnUpdate() override
    {
        Alas::Window::SetCurrentWindow(*_editorWindow);
        Alas::RenderCommand::SetClearColor({ 0.2f, 0.6f, 0.8f, 1 });
        Alas::RenderCommand::Clear();
        
        float deltaTime = Alas::Time::getPhysicsDeltaTime();
        _timeElapsed += deltaTime;
        _framesElapsed += 1;

        if (_timeElapsed > 1.0)
        {
            _frameRate = _framesElapsed / _timeElapsed;
            _framesElapsed = 0;
            _timeElapsed = 0;
        }

        glm::vec2 velocity = Alas::ScriptingEngine::LuaUpdate();
        // _mainGo.GetComponent<Alas::RigidBody2D>().Velocity = velocity;
        auto& transfrom = _mainGo.GetComponent<Alas::Transform>();
        transfrom.Position.x += velocity.x;
        transfrom.Position.y += velocity.y;
        
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

    Alas::Entity OnCreateObjectButton()
    {
        Alas::Entity ent = _scene->CreateEntity("Quad");
        auto& sprite = ent.AddComponent<Alas::SpriteComponent>(_baseTexture, _textureShader);
        auto& transform = ent.GetComponent<Alas::Transform>();
        transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
        transform.Position = glm::vec3(0.0f);
        transform.Rotation = glm::vec3(0.0f);
        // ALAS_CLIENT_INFO("{0} {1} {2}", delta, time, sin(time));
        
        return ent; 
    }

    Alas::Entity& OnAddComponentButton(Alas::Entity& ent, int selectedComponentId)
    {
        if (selectedComponentId >= IM_ARRAYSIZE(_componentsStr)) return ent;

        if (_componentsStr[selectedComponentId] == SPRITE_C && !ent.HasComponent<Alas::SpriteComponent>())
            ent.AddComponent<Alas::SpriteComponent>(_baseTexture, _textureShader);
        else if (_componentsStr[selectedComponentId] == RIGID_BODY_2D_C && !ent.HasComponent<Alas::RigidBody2D>())
            ent.AddComponent<Alas::RigidBody2D>();
        else if (_componentsStr[selectedComponentId] == BOX_COLLIDER_2D_C && !ent.HasComponent<Alas::BoxCollider2D>() ) 
            ent.AddComponent<Alas::BoxCollider2D>();
            
        return ent; 
    }

    void OnImGuiRender()
    {
        
        bool open = true;
        ImGui::ShowDemoWindow(&open);

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

        ImGui::InputText("Scene Name", &_sceneToLoadName);
        if (ImGui::Button("Save scene", ImVec2(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT)))
        {
            Alas::SceneSerialization::SerializeScene(_scene, "Assets/" + _sceneToLoadName + ".yaml");  
        }
        if (ImGui::Button("Load Scene", ImVec2(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT)))
        {
            _scene = Alas::SceneSerialization::DeserializeScene("Assets/" + _sceneToLoadName + ".yaml");  
        }
          
        ImGui::SeparatorText("Game objects");
        if (ImGui::Button("Create game object", ImVec2(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT)))
        {
            OnCreateObjectButton();
        }

        auto objects = _scene->GetEntityMap();
        std::map<Alas::UID, Alas::Entity>::iterator it;      
        for (it = objects.begin(); it != objects.end(); it++)
        {
            Alas::Entity ent = it->second;
            std::string str = "ID: " + std::to_string((ent.GetUID())) + " " + ent.GetComponent<Alas::TagComponent>().Tag;

            static int selectedComponentId = 0; // Here we store our selection data as an index.
            if (ImGui::TreeNode(str.c_str()))
            {
                {                        
                    // Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
                    const char* combo_preview_value = _componentsStr[selectedComponentId];

                    if (ImGui::BeginCombo("Component to add", combo_preview_value))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(_componentsStr); n++)
                        {
                            const bool is_selected = (selectedComponentId == n);
                            if (ImGui::Selectable(_componentsStr[n], is_selected))
                            {
                                selectedComponentId = n;
                            }
                                
                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Spacing();
                    
                }
                
                if (ImGui::Button("Add Component"))
                {
                    OnAddComponentButton(ent, selectedComponentId);
                }

                ImGui::LabelText(ID_C, std::to_string((ent.GetUID())).c_str());

                ImGui::InputText(TAG_C, &ent.GetComponent<Alas::TagComponent>().Tag);

                if(ImGui::TreeNode(TRANSFORM_C))
                {
                    auto& transform = ent.GetComponent<Alas::Transform>();
                    ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), BASE_DRAG_STEP);
                    ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), BASE_DRAG_STEP);
                    ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), BASE_DRAG_STEP);
                    // if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::Transform>();
                    ImGui::TreePop();
                }

                if (ent.HasComponent<Alas::SpriteComponent>())
                if(ImGui::TreeNode(SPRITE_C))
                {
                    auto& sprite = ent.GetComponent<Alas::SpriteComponent>();
                    ImGui::LabelText(SPRITE_C_SHADER, Alas::ResourceManager::GetResourceFilepath(sprite.c_Shader->GetUID()).c_str());
                    ImGui::LabelText(SPRITE_C_TEXTURE, Alas::ResourceManager::GetResourceFilepath(sprite.c_Texture->GetUID()).c_str());
                    ImGui::ColorEdit4("Color", glm::value_ptr(ent.GetComponent<Alas::SpriteComponent>().Color));
                    if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::SpriteComponent>();
                    ImGui::TreePop();
                }

                if (ent.HasComponent<Alas::RigidBody2D>())
                if(ImGui::TreeNode(RIGID_BODY_2D_C))
                {
                    auto& rigidBody2D = ent.GetComponent<Alas::RigidBody2D>();
                                        
                    {
                                                
                        const char* typesStr[] = {
                            RIGID_BODY_2D_TYPE_DYNAMIC_STR,
                            RIGID_BODY_2D_TYPE_KINEMATIC_STR,
                            RIGID_BODY_2D_TYPE_STATIC_STR
                        };

                        const Alas::RigidBody2D::BodyType types[] = {
                            Alas::RigidBody2D::BodyType::Dynamic,
                            Alas::RigidBody2D::BodyType::Kinematic,
                            Alas::RigidBody2D::BodyType::Static,
                        };

                        static int item_selected_idx = 0; // Here we store our selection data as an index.

                        // Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
                        const char* combo_preview_value = typesStr[item_selected_idx];

                        if (ImGui::BeginCombo("Type", combo_preview_value))
                        {
                            for (int n = 0; n < IM_ARRAYSIZE(typesStr); n++)
                            {
                                const bool is_selected = (item_selected_idx == n);
                                if (ImGui::Selectable(typesStr[n], is_selected))
                                {
                                    item_selected_idx = n;
                                    rigidBody2D.Type = types[item_selected_idx];
                                }
                                    
                                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::Spacing();
                    }
                    
                    ImGui::DragFloat(RIGID_BODY_2D_C_MASS, &rigidBody2D.Mass, BASE_DRAG_STEP);
                    ImGui::DragFloat(RIGID_BODY_2D_C_GRAVITY_SCALE, &rigidBody2D.GravityScale, BASE_DRAG_STEP);

                    if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::RigidBody2D>();
                    ImGui::TreePop();
                }

                if (ent.HasComponent<Alas::BoxCollider2D>())
                if(ImGui::TreeNode(BOX_COLLIDER_2D_C))
                {
                    auto& boxCollider2D = ent.GetComponent<Alas::BoxCollider2D>();
                    ImGui::DragFloat2(BOX_COLLIDER_2D_C_OFFSET, glm::value_ptr(boxCollider2D.Offset), BASE_DRAG_STEP);
                    ImGui::DragFloat2(BOX_COLLIDER_2D_C_SIZE, glm::value_ptr(boxCollider2D.Size), BASE_DRAG_STEP);
                    if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::BoxCollider2D>();
                    ImGui::TreePop();
                }

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

        const char* _componentsStr[3] = {
            SPRITE_C,
            RIGID_BODY_2D_C,
            BOX_COLLIDER_2D_C
        };

        std::string _sceneToLoadName;

        Alas::Window* _editorWindow;

        Alas::Shared<Alas::Scene> _scene;

        Alas::Entity _mainGo;
        
        Alas::Shared<Alas::Shader> _textureShader;

        Alas::Shared<Alas::Texture> _baseTexture;
        Alas::Shared<Alas::Texture> _mainGOTexture;

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
