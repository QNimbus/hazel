#include "hzpch.h"
#include "hazel/renderer/GraphicsContext.h"

#include "hazel/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLContext.h"

namespace Hazel {
	Scope<GraphicsContext> GraphicsContext::Create(void* window) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: {
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported)");
				return nullptr;
				break;
			}
			case RendererAPI::API::OpenGL: {
				return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
				break;
			}
			default: {
				return nullptr;
				break;
			}
		}
	}
}