#pragma once

// Headers for creating custom gameobjects
#include "Core.h"

// Utils
#include "src/Utils/Profiler.h"

// core app and imgui layer 
#include "src/Core/Application.h"
#include "src/ImGui/ImGuiLayer.h"

// Scene loading and saving
#include "src/Entity/SceneSerialization.h"

// Resources Management
#include "src/Resources/ResourceManager.h"

// Renderer
#include "src/Renderer/Renderer.h"
#include "src/Renderer/RendererCommand.h"
#include "src/Renderer/Shader.h"
#include "src/Renderer/VertexBuffer.h"
#include "src/Renderer/IndexBuffer.h"
#include "src/Renderer/VertexArray.h"
#include "src/Renderer/OrthCamera.h"
#include "src/Renderer/Texture.h"

// ENTRY POINT
#include "src/Core/EntryPoint.h"
