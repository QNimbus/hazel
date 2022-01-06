#include "hzpch.h"
#include "hazel/renderer/FrameBuffer.h"

#include "hazel/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLFrameBuffer.h"

namespace Hazel {

	Ref<FrameBuffer> Hazel::FrameBuffer::Create(const FrameBufferSpecification& spec) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported)");
			return nullptr;
		}
		case RendererAPI::API::OpenGL: {
			return CreateRef<OpenGLFrameBuffer>(spec);
		}
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}


