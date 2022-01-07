#pragma once

// For use by client applications that want to use the engine

#include "hazel/core/Application.h"
#include "hazel/core/Layer.h"
#include "hazel/core/Log.h"

#include "hazel/core/Timestep.h"

#include "hazel/core/Input.h"
#include "hazel/core/KeyCodes.h"
#include "hazel/core/MouseCodes.h"

#include "hazel/imgui/ImGuiLayer.h"

// -- Renderer    ---------

#include "hazel/renderer/Renderer.h"
#include "hazel/renderer/Renderer2D.h"
#include "hazel/renderer/RenderCommand.h"

#include "hazel/renderer/Buffer.h"
#include "hazel/renderer/Shader.h"
#include "hazel/renderer/Texture.h"
#include "hazel/renderer/Framebuffer.h"
#include "hazel/renderer/SubTexture2D.h"
#include "hazel/renderer/VertexArray.h"
#include "hazel/renderer/CameraController.h"

#include "hazel/renderer/Camera.h"