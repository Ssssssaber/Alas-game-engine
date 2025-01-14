#include <Alas.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/string_cast.hpp"
#include <gtc/type_ptr.hpp>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <imgui_internal.h>

Alas::VertexArray* GenerateTriangleVertexArray()
{
     /*
        TRIANGLE VERTEX ARRAY
    */
    Alas::VertexArray* triVertexArray = Alas::VertexArray::Create();

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,
        0.0f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 0.5f
    };
    
    Alas::Shared<Alas::VertexBuffer> triVertexBuffer; 
    triVertexBuffer.reset(Alas::VertexBuffer::Create(vertices, sizeof(vertices)));
    
    {
        Alas::BufferLayout layout {
            {Alas::ShaderElementType::Float3, "a_Position"},
            {Alas::ShaderElementType::Float4, "a_Color"}
        };
        triVertexBuffer->SetLayout(layout);
    }

    triVertexArray->AddVertexBuffer(triVertexBuffer);
    
    uint32_t triIndices[3] = { 0, 1, 2 };        
    Alas::Shared<Alas::IndexBuffer> triIndexBuffer;
    triIndexBuffer.reset(Alas::IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t)));
    
    triVertexArray->SetIndexBuffer(triIndexBuffer);

    return triVertexArray;
}

Alas::VertexArray* GenerateQuadVertexArray()
{
    /*
        QUAD VERTEX ARRAY
    */
    Alas::VertexArray* quadVertexArray = Alas::VertexArray::Create();

    float quadVertices[3 * 4] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };
    
    Alas::Shared<Alas::VertexBuffer> quadVertexBuffer; 
    quadVertexBuffer.reset(Alas::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
    
    {
        Alas::BufferLayout layout {
            {Alas::ShaderElementType::Float3, "a_Position"}
        };
        quadVertexBuffer->SetLayout(layout);
    }

    quadVertexArray->AddVertexBuffer(quadVertexBuffer);
    
    uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };        
    Alas::Shared<Alas::IndexBuffer> quadIndexBuffer;
    quadIndexBuffer.reset(Alas::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
    
    quadVertexArray->SetIndexBuffer(quadIndexBuffer);

    return quadVertexArray;
}

Alas::Shared<Alas::Shader> GenerateBaseShader()
{
    /*
        BASE SHADER
    */

    std::string baseVertexSrc = R"(
        #version 330 core
        
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        
        out vec3 o_Position;
        out vec4 o_Color;

        uniform mat4 u_viewProjectionMatrix;
        uniform mat4 u_model;

        void main()
        {
            o_Position = a_Position;
            o_Color = a_Color;
            gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position, 1.0);	
        }
    )";

    std::string baseFragmentSrc = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 o_Position;
        in vec4 o_Color;

        uniform vec4 u_Color;

        void main()
        {
            color = u_Color;
        }
    )";     

    return Alas::Shader::Create(baseVertexSrc, baseFragmentSrc);
}

class ExampleLayer : public Alas::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        Alas::Application::Get().GetWindow().SetVSync(false);

        float height = Alas::Application::Get().GetWindow().GetHeight();
        float width = Alas::Application::Get().GetWindow().GetWidth();
        
        _camera.reset(new Alas::OrthCamera(-1.6f, 1.6f, -0.9f, 0.9f));
        _scene.reset(new Alas::Scene());

        _triangleVertexArray.reset(GenerateTriangleVertexArray());
        _quadVertexArray.reset(GenerateQuadVertexArray());

        _baseShader = Alas::Shader::Create("Assets/Shaders/BaseShader.shader");
        // _baseShader = GenerateBaseShader();

        _triangle.reset(new Alas::GameObject(_triangleVertexArray, _baseShader, "main triangle"));
        _triangle->SetColor(glm::vec3(0.3f, 0.9f, 0.6f));

        for (int i = 0; i < 40; i++)
        {
            for (int j = 0; j < 40; j++)
            {
                Alas::Shared<Alas::GameObject> go;
                go.reset(new Alas::GameObject(_quadVertexArray, _baseShader, "quad " + char(1)));
                int a = i + 1, b = j + 1;
                glm::vec4 color = glm::normalize(glm::vec4(a, b, glm::abs(a - b), a + b));
                go->SetColor(color);
                go->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
                go->SetPosition(glm::vec3(i * 0.3f, j * 0.3f, 0.0f));
                
                _scene->AddGameObject(go);
                // _scene->_sceneDict.insert(std::make_pair(i + j, go));
            }
        }


        _scene->AddGameObject(_triangle);
        
    }

    void OnUpdate() override
    {           
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

        // if (Alas::Input::IsKeyPressed(ALAS_KEY_I))
        // {
        //     _tri_pos.y += triangleSpeed * deltaTime;
        // }
        // if (Alas::Input::IsKeyPressed(ALAS_KEY_K))
        // {
        //     _tri_pos.y -= triangleSpeed * deltaTime;
        // }
        // if (Alas::Input::IsKeyPressed(ALAS_KEY_J))
        // {
        //     _tri_pos.x -= triangleSpeed * deltaTime;
        // }
        // if (Alas::Input::IsKeyPressed(ALAS_KEY_L))
        // {
        //     _tri_pos.x += triangleSpeed * deltaTime;
        // }

        // triangle->SetPosition(_tri_pos);

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

        // ALAS_CLIENT_INFO("{0}; {1}", glm::to_string(triangle->GetPosition()), glm::to_string(go->GetPosition()));
        std::map<uint64_t, Alas::Shared<Alas::GameObject>> objects = _scene->getGameObjectList();
        std::map<uint64_t, Alas::Shared<Alas::GameObject>>::iterator it;
        
        for (it = objects.begin(); it != objects.end(); it++)
        {
            // TODO :: SORTING GAMEOBJECTS BY Z COORDINATE
            Alas::Shared<Alas::GameObject> go = it->second;
            go->Update();
            Alas::Renderer::Submit(go);
            // Alas::Renderer::Submit(go->GetVertexArray(), go->GetShader(), go->GetModelMatrix());
        }
        
        Alas::Renderer::EndScene();
    }

    Alas::Shared<Alas::GameObject> OnCreateObjectButton(const Alas::Shared<Alas::VertexArray>& vertexArray, const Alas::Shared<Alas::Shader>& shader)
    {
        Alas::Shared<Alas::GameObject> go;
        go.reset(new Alas::GameObject(vertexArray, shader));
        float delta = Alas::Time::getDeltaTime();
        float time = Alas::Time::GetTimeInSeconds();
        ALAS_CLIENT_INFO("{0} {1} {2}", delta, time, sin(time));
        go->SetRotation(glm::vec3(0.0f, 0.0f, time * 10));
        go->SetPosition(glm::vec3(delta * 20 + time / 250, delta * 20 - time / 250, 0.0f));
        glm::vec3 color = glm::vec3(abs(glm::sin(time)));
        go->GetShader()->setVec4("u_Color", color.x, color.y, color.z, 1.0f);
        ALAS_CLIENT_INFO("{0}", glm::to_string(color));
        _scene->AddGameObject(go);
        return go;
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
        ImGui::InputFloat("FPS", &_frameRate);
        if (ImGui::Button("Create triangle", ImVec2(200, 50)))
        {
            OnCreateObjectButton(_triangleVertexArray, _baseShader);   
        }
        if (ImGui::Button("Create quad", ImVec2(200, 50)))
        {
            OnCreateObjectButton(_quadVertexArray, _baseShader);
        }

        ImGui::SeparatorText("Game objects");

        std::map<uint64_t, Alas::Shared<Alas::GameObject>> objects = _scene->getGameObjectList();
        std::map<uint64_t, Alas::Shared<Alas::GameObject>>::iterator it;      
        for (it = objects.begin(); it != objects.end(); it++)
        {
            Alas::Shared<Alas::GameObject> go = it->second;
            std::string str = "ID: " + std::to_string(go->GetId()) + " " + *go->GetName();
            if (ImGui::TreeNode(str.c_str()))
            {
                ImGui::InputText("Name", go->GetName());
                ImGui::DragFloat3("Position", glm::value_ptr(go->GetPosition()), 0.01f);
                ImGui::DragFloat3("Rotation", glm::value_ptr(go->GetRotation()));
                ImGui::DragFloat3("Scale", glm::value_ptr(go->GetScale()), 0.01f);
                ImGui::ColorEdit3("Color", glm::value_ptr(go->GetColor()));
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

        Alas::Shared<Alas::Scene> _scene;

        Alas::Shared<Alas::GameObject> _triangle;

        Alas::Shared<Alas::VertexArray> _triangleVertexArray;
        Alas::Shared<Alas::VertexArray> _quadVertexArray;
        
        Alas::Shared<Alas::Shader> _baseShader;

        glm::vec3 _quadColor = { 0.5f, 0.1f, 0.3f};
        glm::vec3 _triColor = { 0.2f, 0.6f, 0.8f};

        float _triangleSpeed = 2.0f;
        float _triangleRotationSpeed = 1.0f;

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
        // PushOverlay(new AGS::ImGuiLayer());
    }

    ~Sandbox()
    {

    }
};


Alas::Application* Alas::CreateApplication()
{
    return new Sandbox();
}
