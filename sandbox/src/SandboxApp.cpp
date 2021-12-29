#include <Hazel.h>

#include "platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//using namespace Hazel;

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f) {
		
			{
				m_VertexArray.reset(Hazel::VertexArray::Create());

				float vertices[3 * 6] = {
					-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f,
					 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f,
					 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f,
				};

				Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
				Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
				vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

				vertexBuffer->SetLayout({
				{ Hazel::ShaderDataType::Float3, "a_Position", false },
				{ Hazel::ShaderDataType::Float3, "a_Color", false },
					});
				m_VertexArray->AddVertexBuffer(vertexBuffer);

				uint32_t indices[3] = {
					0, 1, 2
				};
				indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
				m_VertexArray->SetIndexBuffer(indexBuffer);
			}

			{
				m_SquareVertexArray.reset(Hazel::VertexArray::Create());

				float squareVertices[5 * 4] = {
					-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
					 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
					 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
					-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
				};

				Hazel::Ref<Hazel::VertexBuffer> squareVB;
				squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
				squareVB->SetLayout({
					{ Hazel::ShaderDataType::Float3, "a_Position" },
					{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
					});
				m_SquareVertexArray->AddVertexBuffer(squareVB);

				uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
				Hazel::Ref<Hazel::IndexBuffer> squareIB;
				squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
				m_SquareVertexArray->SetIndexBuffer(squareIB);
			}

		std::string vertexSrc = R"(
			#version 450 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Color = vec4(a_Color, 1.0f);
				v_Position = a_Position * 0.5 + 0.5;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 450 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = v_Color;
			}
		)";

		m_Shader = Hazel::Shader::Create(vertexSrc, fragmentSrc);

		m_FlatShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");


		m_TextureShader = Hazel::Shader::Create("assets/shaders/Texture.glsl");
		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_TextureLogo = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override {
		//HZ_TRACE("Delta time: {0}s {1}ms", ts.GetSeconds(), ts.GetMilliSeconds());

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Hazel::RenderCommand::SetClearColor({ 0.07f, 0.07f, 0.07f, 1.0f });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->UploadUniformFloat3("u_Color", m_SquareColor);	

		for (int y = 0; y < 20; y += 1) {
			for (int x = 0; x < 20; x += 1) {
				glm::vec3 pos({ x * 0.11f, y * 0.11f, 0.0f });
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;				
				Hazel::Renderer::Submit(m_FlatShader, m_SquareVertexArray, transform);
			}
		}

		// Large square
		m_Texture->Bind();		
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Transparent logo
		m_TextureLogo->Bind();		
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();

		m_VertexArray->Bind();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	bool OnKeyPressedEven(Hazel::KeyPressedEvent& event) {
		int keycode = event.GetKeyCode();
		return false;
	}

	void OnEvent(Hazel::Event& event) {
		//HZ_TRACE("{0}", event);

		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEven));
	};

private:
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::Shader> m_FlatShader;
	Hazel::Ref<Hazel::Shader> m_TextureShader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_TextureLogo;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 2.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}