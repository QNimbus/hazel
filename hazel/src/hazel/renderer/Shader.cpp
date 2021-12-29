#include "hzpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

#include <fstream>

namespace Hazel {
	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported)");
			return nullptr;
		}
		case RendererAPI::API::OpenGL: {
			return std::make_shared<OpenGLShader>(filepath);
		}
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: {
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported)");
			return nullptr;
		}
		case RendererAPI::API::OpenGL: {
			return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
		}
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	std::string Shader::ReadFile(const std::string& filepath) const {
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
		}
		else {
			HZ_CORE_ERROR("Could not open file: '{0}'", filepath);
		}

		return result;
	}
}
