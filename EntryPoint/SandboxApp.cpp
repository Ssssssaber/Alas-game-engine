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

#define BASE_DRAG_STEP 1.0f
#define SLOW_DRAG_STEP 0.1f
class ExampleLayer : public Alas::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        // Alas::ScriptingEngine::LoadAndExecuteScript("main-go.lua");

        _editorWindow = &Alas::Application::Get().GetWindow(); 
        _editorWindow->SetVSync(true);
        
        // _camera.reset(new Alas::OrthCamera(-1.6f, 1.6f, -0.9f, 0.9f));        
        _scene.reset(new Alas::Scene());
        
        // -------------- test scene load correctly

        _mainGOTexture = Alas::ResourceManager::GetTexture("Assets/Textures/keke.png");

        _mainGo = _scene->CreateEntity("Main triangle");
        // auto& script = _mainGo.AddComponent<Alas::NativeScriptComponent>();
        // script.Bind<Triangle>();

        auto& sprite = _mainGo.AddComponent<Alas::SpriteComponent>(_mainGOTexture, Alas::ResourceManager::GetBaseShader());
        sprite.Color = glm::vec4(1.0f);

        _mainGo.GetComponent<Alas::Transform>().Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

        auto& body = _mainGo.AddComponent<Alas::RigidBody2D>(Alas::RigidBody2D::BodyType::Dynamic);
        body.GravityScale = 0;
        _mainGo.AddComponent<Alas::BoxCollider2D>();

        auto& luaScript = _mainGo.AddComponent<Alas::LuaScriptComponent>("Assets/Scripts/main-test.lua");
        ALAS_CORE_INFO(luaScript.Filepath);

        auto& text = _mainGo.AddComponent<Alas::WorldSpaceText>("keke");
        text.Offset = glm::vec2(1.0f);


        Alas::Entity overlay = _scene->CreateEntity("overlay text");
        auto textOverlay = overlay.AddComponent<Alas::OverlayText>("keke wa baba", glm::vec4(0.5f));

        
        Alas::Entity worldSpace = _scene->CreateEntity("world space text");
        auto textWorldSpace = worldSpace.AddComponent<Alas::WorldSpaceText>("keke", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        
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

        Alas::FramebufferSpecification spec;
        spec.Width = 1270;
        spec.Height = 720;
		m_Framebuffer = Alas::Framebuffer::Create(spec);

        _camera.reset(new Alas::OrthCamera(spec.Width, spec.Height));
        
        // _cameraPos = glm::vec3(-500.0f, -500.0f, 0.0f);
        // _camera->SetPosition(_cameraPos);
    }

    void OnUpdate() override
    {
        ALAS_PROFILE_FUNCTION();
        Alas::Window::SetCurrentWindow(*_editorWindow);
        Alas::RenderCommand::SetClearColor({ 0.2f, 0.6f, 0.8f, 1 });
        Alas::RenderCommand::Clear();


        if (Alas::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			// _camera->SetNewSize(m_ViewportSize.x, m_ViewportSize.y);
		}

        m_Framebuffer->Bind();
        Alas::RenderCommand::SetClearColor({ 0.6f, 0.6f, 0.8f, 1 });
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

        if (m_ViewportFocused)
		{
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

            // if (_entityHovered && _entitySelected &&
            //     _hoveredEntity.GetUID() == _selectedEntity.GetUID() &&
            //     Alas::Input::IsMouseButtonPressed(ALAS_MOUSE_BUTTON_LEFT))
            // {
            //     auto& transform = _selectedEntity.GetComponent<Alas::Transform>();
            //     auto[mx, my] = ImGui::GetMousePos();
            //     mx -= m_ViewportBounds[0].x;
            //     my -= m_ViewportBounds[0].y;
            //     glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
            //     my = viewportSize.y - my;
            //     transform.Position.x = mx;
            //     transform.Position.y = mx;
            // }
        }
        
        Alas::Renderer::BeginScene(_camera);

        _scene->SceneUpdate();
        _hoveredEntity = GetEntityUnderCursor();
        if (_hoveredEntity)
        {
            auto& transform = _hoveredEntity->GetComponent<Alas::Transform>();
            Alas::Renderer::DrawBox(transform.Position, transform.Rotation.z, transform.Scale, glm::vec4(0.0f, 1.0f, 0.0f, 0.8f));
        }
        
        if (Alas::Input::IsMouseButtonPressed(ALAS_MOUSE_BUTTON_LEFT))
        {
            _selectedEntity = GetEntityUnderCursor();

        }
        
        if (_selectedEntity)
        {
            auto& transform = _selectedEntity->GetComponent<Alas::Transform>();
            Alas::Renderer::DrawBox(transform.Position, transform.Rotation.z, transform.Scale, glm::vec4(0.0f, 1.0f, 0.0f, 0.8f));
        }  

        Alas::Renderer::EndScene();

        m_Framebuffer->Unbind();
    }

    Alas::Entity OnCreateObjectButton()
    {
        Alas::Entity ent = _scene->CreateEntity("Quad");
        auto& sprite = ent.AddComponent<Alas::SpriteComponent>(Alas::ResourceManager::GetBaseTexture(), Alas::ResourceManager::GetBaseShader());
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
            ent.AddComponent<Alas::SpriteComponent>(Alas::ResourceManager::GetBaseTexture(), Alas::ResourceManager::GetBaseShader());
        else if (_componentsStr[selectedComponentId] == RIGID_BODY_2D_C && !ent.HasComponent<Alas::RigidBody2D>())
            ent.AddComponent<Alas::RigidBody2D>();
        else if (_componentsStr[selectedComponentId] == BOX_COLLIDER_2D_C && !ent.HasComponent<Alas::BoxCollider2D>() ) 
            ent.AddComponent<Alas::BoxCollider2D>();
        else if (_componentsStr[selectedComponentId] == LUA_SCRIPT_C && !ent.HasComponent<Alas::LuaScriptComponent>() ) 
            ent.AddComponent<Alas::LuaScriptComponent>();
        else if (_componentsStr[selectedComponentId] == OVERLAY_TEXT_C && !ent.HasComponent<Alas::OverlayText>() ) 
            ent.AddComponent<Alas::OverlayText>();
        else if (_componentsStr[selectedComponentId] == WORLD_SPACE_TEXT_C && !ent.HasComponent<Alas::WorldSpaceText>() ) 
            ent.AddComponent<Alas::WorldSpaceText>();
        else if (_componentsStr[selectedComponentId] == CAMERA_C && !ent.HasComponent<Alas::CameraComponent>() ) 
            ent.AddComponent<Alas::CameraComponent>();
        return ent; 
    }

    void OnImGuiRender()
    {
        ALAS_PROFILE_FUNCTION();
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

        ImGui::Begin("Scene Tree");
        
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
        if (ImGui::Button("Refresh ResourceManager", ImVec2(BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT)))
        {
            Alas::ResourceManager::UpdateMetaFiles();
        }

        ImGui::SeparatorText("COLLIDERS");
        ImGui::DragFloat("Box scale", &_scene->BOX_PHYSICS_SCALE, BASE_DRAG_STEP);
        ImGui::Checkbox("Draw Colliders", &_scene->DrawColliders);
        ImGui::SeparatorText("GRAVITY");
        ImGui::DragFloat2("Gravity", glm::value_ptr(_scene->_gravity), BASE_DRAG_STEP);

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
            
            if (_selectedEntity && ent.GetUID() == _selectedEntity->GetUID())
            {
                if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Selected))
                {   
                    ImGuiRenderEntityComponents(ent);
                    ImGui::TreePop();
                }
            }
            else
            {
                if (ImGui::TreeNodeEx(str.c_str()))
                {
                    ImGuiRenderEntityComponents(ent);
                    ImGui::TreePop();
                }    
            }

        }

        ImGui::End();


        ImGui::Begin("Scene Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
 
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachment();
		ImGui::Image(textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, {0, 1}, {1, 0});

        ImGui::End();
    }

    void ImGuiRenderEntityComponents(Alas::Entity ent)
    {
        static int selectedComponentId = 0; // Here we store our selection data as an index.
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

            ImGui::LabelText(SPRITE_C_SHADER, Alas::ResourceManager::GetResourceFilepathString(sprite.c_Shader->GetUID()).c_str());
            {
                static int selectedShaderID = 0;
                std::vector<std::string> shaderFiles = Alas::ResourceManager::GetFilesWithExtension(".shader");
                const char* combo_preview_value = shaderFiles[selectedShaderID].c_str();

                if (ImGui::BeginCombo("Shaders", combo_preview_value))
                {
                    for (int n = 0; n < shaderFiles.size(); n++)
                    {
                        const bool is_selected = (selectedShaderID == n);
                        if (ImGui::Selectable(shaderFiles[n].c_str(), is_selected))
                        {
                            selectedShaderID = n;
                        }
                            
                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::Button("Change Shader"))
                {
                    sprite.c_Shader = Alas::ResourceManager::GetShader(shaderFiles[selectedShaderID]);
                }
            }
            

            ImGui::LabelText(SPRITE_C_TEXTURE, Alas::ResourceManager::GetResourceFilepathString(sprite.c_Texture->GetUID()).c_str());
            {
                static int selectedTextureID = 0;
                std::vector<std::string> textureFiles = Alas::ResourceManager::GetFilesWithExtension(".png");
                const char* combo_preview_value = textureFiles[selectedTextureID].c_str();

                if (ImGui::BeginCombo("Textures", combo_preview_value))
                {
                    for (int n = 0; n < textureFiles.size(); n++)
                    {
                        const bool is_selected = (selectedTextureID == n);
                        if (ImGui::Selectable(textureFiles[n].c_str(), is_selected))
                        {
                            selectedTextureID = n;
                        }
                            
                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::Button("Change Texture"))
                {
                    sprite.c_Texture = Alas::ResourceManager::GetTexture(textureFiles[selectedTextureID]);
                }
            }
            
            ImGui::ColorEdit4("Color", glm::value_ptr(ent.GetComponent<Alas::SpriteComponent>().Color));
            if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::SpriteComponent>();
            ImGui::TreePop();
        }

        if (ent.HasComponent<Alas::LuaScriptComponent>())
        if(ImGui::TreeNode(LUA_SCRIPT_C))
        {
            auto& lua = ent.GetComponent<Alas::LuaScriptComponent>();
            ImGui::LabelText(LUA_SCRIPT_C_FILE, (lua.Filepath).c_str());

            static int selectedScriptID = 0;
            std::vector<std::string> luaFiles = Alas::ResourceManager::GetFilesWithExtension(".lua");
            const char* combo_preview_value = luaFiles[selectedScriptID].c_str();

            if (ImGui::BeginCombo("Lua scripts", combo_preview_value))
            {
                for (int n = 0; n < luaFiles.size(); n++)
                {
                    const bool is_selected = (selectedScriptID == n);
                    if (ImGui::Selectable(luaFiles[n].c_str(), is_selected))
                    {
                        selectedScriptID = n;
                    }
                        
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Change script"))
            {
                lua.Filepath = luaFiles[selectedScriptID];
            }

            if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::LuaScriptComponent>();
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

        if (ent.HasComponent<Alas::OverlayText>())
        if(ImGui::TreeNode(OVERLAY_TEXT_C))
        {
            auto& overlayText = ent.GetComponent<Alas::OverlayText>();
            ImGui::InputText(OVERLAY_TEXT_C_DISPLAY_TEXT, &overlayText.DisplayText);
            ImGui::DragFloat2(OVERLAY_TEXT_C_SCREEN_POSITION, glm::value_ptr(overlayText.ScreenPosition), BASE_DRAG_STEP);
            ImGui::DragFloat(OVERLAY_TEXT_C_ROTATION, &overlayText.Rotation, BASE_DRAG_STEP);
            ImGui::DragFloat2(OVERLAY_TEXT_C_SCALE, glm::value_ptr(overlayText.Scale), BASE_DRAG_STEP);
            ImGui::DragFloat4(OVERLAY_TEXT_C_COLOR, glm::value_ptr(overlayText.Color), BASE_DRAG_STEP);
            
            if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::OverlayText>();
            ImGui::TreePop();
        }

        
        if (ent.HasComponent<Alas::WorldSpaceText>())
        if(ImGui::TreeNode(WORLD_SPACE_TEXT_C))
        {
            auto& worldSpaceText = ent.GetComponent<Alas::WorldSpaceText>();
            ImGui::InputText(WORLD_SPACE_TEXT_C_DISPLAY_TEXT, &worldSpaceText.DisplayText);
            ImGui::DragFloat2(WORLD_SPACE_TEXT_C_OFFSET, glm::value_ptr(worldSpaceText.Offset), BASE_DRAG_STEP);
            ImGui::DragFloat(WORLD_SPACE_TEXT_C_ROTATION, &worldSpaceText.Rotation, BASE_DRAG_STEP);
            ImGui::DragFloat2(WORLD_SPACE_TEXT_C_SCALE, glm::value_ptr(worldSpaceText.Scale), BASE_DRAG_STEP);
            ImGui::DragFloat4(WORLD_SPACE_TEXT_C_COLOR, glm::value_ptr(worldSpaceText.Color), BASE_DRAG_STEP);
            
            if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::WorldSpaceText>();
            ImGui::TreePop();
        }

        if (ent.HasComponent<Alas::CameraComponent>())
        if(ImGui::TreeNode(CAMERA_C))
        {
            auto& camera = ent.GetComponent<Alas::CameraComponent>();
            ImGui::DragFloat2(CAMERA_C_OFFSET, glm::value_ptr(camera.Offset), BASE_DRAG_STEP);
            if (ImGui::Button("Remove Component")) ent.RemoveComponent<Alas::WorldSpaceText>();
            ImGui::TreePop();
        }


        if (ImGui::Button("Delete entity", { BASE_BUTTON_WIDTH, BASE_BUTTON_HEIGHT}))
        {
            _scene->DeleteEntityWithId(ent.GetUID());
        }
    }

    void OnEvent(Alas::Event& event) override
    {
        
    }

    Alas::Entity* GetEntityUnderCursor()
    {
        auto[mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			uint32_t id = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            return _scene->GetEntityByIdIfExists(id);
		}

        return false;
    }

    private:
        float _timeElapsed = 0;
        float _framesElapsed = 0;
        float _frameRate = 0;

        const char* _componentsStr[7] = {
            SPRITE_C,
            RIGID_BODY_2D_C,
            BOX_COLLIDER_2D_C,
            LUA_SCRIPT_C,
            OVERLAY_TEXT_C,
            WORLD_SPACE_TEXT_C,
            CAMERA_C
        };
        Alas::Shared<Alas::Framebuffer> m_Framebuffer;

        std::string _sceneToLoadName;

        Alas::Window* _editorWindow;

        Alas::Shared<Alas::Scene> _scene;

        Alas::Entity _mainGo;
        Alas::Shared<Alas::Texture> _mainGOTexture;

        Alas::Shared<Alas::OrthCamera> _camera;
        glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
        float _cameraRotation = 0;

        float _cameraSpeed = 100.0f;
        float _cameraRotationSpeed = 15.0f;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

        Alas::Entity* _selectedEntity = nullptr;
        Alas::Entity* _hoveredEntity = nullptr;
};

class Sandbox : public Alas::Application
{
public:
    Sandbox()
    {
        ALAS_PROFILE_BEGIN_SESSION("profiling");
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {
        ALAS_PROFILE_END_SESSION();
    }
};


Alas::Application* Alas::CreateApplication()
{
    return new Sandbox();
}
