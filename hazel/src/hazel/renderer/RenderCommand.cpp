#include "hzpch.h"
#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}