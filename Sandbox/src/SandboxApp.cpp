#include "Hurikan.h"

#include "imgui.h"

#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox2D.h"
#include "./game/src/ChessApp.h"

class ExampleLayer : public Hurikan::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f,false)
	{
		m_VertexArray = Hurikan::VertexArray::Create();

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,   1.0f,0.0f,0.4f,1.0f,
			 0.5f, -0.5f, 0.0f,   1.0f,0.5f,0.4f,1.0f,
			 0.0,  0.5f, 0.0f,   1.0f,0.0f,0.4f,1.0f,
		};

		unsigned int indices[3]{
			0,1,2,
		};
		Hurikan::Ref<Hurikan::VertexBuffer> vertexBuffer;
		Hurikan::Ref<Hurikan::IndexBuffer> indexBuffer;
		vertexBuffer = Hurikan::VertexBuffer::Create(vertices, sizeof(vertices));
		indexBuffer = Hurikan::IndexBuffer::Create(indices, 3);

		Hurikan::BufferLayout layout = {
			{Hurikan::ShaderDataType::Float3,"a_Position"},
			
		};
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_VertexArray->Unbind();
		
		m_SquareVA = Hurikan::VertexArray::Create();
		Hurikan::Ref<Hurikan::VertexBuffer> squareVB;
		Hurikan::Ref<Hurikan::IndexBuffer> squareIB;

		float square_vertices[] = {
			-0.5, -0.5f, 0.0f,  /*texCoors*/  0.0f, 0.0f,
			 0.5, -0.5f, 0.0f,  /*texCoors*/  1.0f, 0.0f,
			 0.5,  0.5f, 0.0f,  /*texCoors*/  1.0f, 1.0f,
			-0.5,  0.5f, 0.0f,  /*texCoors*/  0.0f, 1.0f
		};

		Hurikan::BufferLayout square_layout = {
			{Hurikan::ShaderDataType::Float3,"a_Position"},
			{Hurikan::ShaderDataType::Float2,"a_TexCoord"},
		};

		squareVB = Hurikan::VertexBuffer::Create(square_vertices, sizeof(square_vertices));
		squareVB->SetLayout(square_layout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int square_indices[6]{
				0,1,2,
				2,3,0
		};

		squareIB = Hurikan::IndexBuffer::Create(square_indices, 6);

		m_SquareVA->SetIndexBuffer(squareIB);
//-----------------------------------------------------------------------------
//------ DEFAULT SHADER -------------------------------------------------------
//-----------------------------------------------------------------------------

		std::string vertexSource = R"(
		#version 330 core

		layout(location = 0)in vec3 a_Position;
		layout(location = 1)in vec2 a_TexCoord;

		uniform mat4 u_ViewProjection;		
		uniform mat4 u_Transform;

		out vec3 v_Position;

		void main() 
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
		}
		)";


		std::string fragSource = R"(
		#version 330 core

		layout(location = 0)out vec4 a_Color;
		
		in vec3 v_Position;
		uniform vec4 u_Color;			

		void main() 
		{
			a_Color = vec4(v_Position,1.0);
			a_Color = u_Color;
		}
		)";

		
		m_Shader = Hurikan::Shader::Create("Default Shader", vertexSource, fragSource);

//-----------------------------------------------------------------------------
//------ TEXTURE SHADER -------------------------------------------------------
//-----------------------------------------------------------------------------
		
		m_Texture = Hurikan::Texture2D::Create("assets/textures/test_texture.png");
		m_TransparentBackgroundTexture = Hurikan::Texture2D::Create("assets/textures/c.png");

		Hurikan::Ref<Hurikan::Shader> m_TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_TextureShader = Hurikan::Shader::Create("assets/shaders/Texture.glsl");

		std::dynamic_pointer_cast<Hurikan::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hurikan::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}
	float size_of_gap = 0.11f;
	void OnUpdate(Hurikan::Timestep deltaTime) override
	{
		//---CAMERA MOVEMENT-------------------------------------------
		m_CameraController.OnUpdate(deltaTime);
		//---TRANSFORM-------------------------------------------
		if (Hurikan::Input::IsKeyPressed(Hurikan::Key::M)) {
			size_of_gap += 0.01f;
		}
		if (Hurikan::Input::IsKeyPressed(Hurikan::Key::N)) {
			size_of_gap -= 0.01f;
		}
		Hurikan::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hurikan::RenderCommand::Clear();

		Hurikan::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<Hurikan::OpenGLShader>(m_Shader)->Bind();
 		for (int y = 0;y < 20; y++)
 		{
 			for (int x = 0;x < 20;x++)
 			{
				glm::vec3 pos(x *size_of_gap, y * size_of_gap, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				std::dynamic_pointer_cast<Hurikan::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color",m_SquareColor);

				Hurikan::Renderer::Submit(m_SquareVA, m_Shader,transform);

			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Hurikan::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f),glm::vec3(1.5f)));

		m_TransparentBackgroundTexture->Bind();
		Hurikan::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//Triangle
		//Hurikan::Renderer::Submit(m_VertexArray, m_Shader);
		Hurikan::Renderer::EndScene();
	}

	void OnEvent(Hurikan::Event& e) override {
		m_CameraController.OnEvent(e);
	}


	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color",glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Hurikan::ShaderLibrary m_ShaderLibrary;
	Hurikan::Ref<Hurikan::Shader> m_Shader;
	Hurikan::Ref<Hurikan::VertexArray> m_VertexArray;

	Hurikan::Ref<Hurikan::VertexArray> m_SquareVA;

	Hurikan::Ref<Hurikan::Texture2D> m_Texture,m_TransparentBackgroundTexture;

	Hurikan::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.2f,0.3f,0.7f,1.0f };
};

class Sandbox : public Hurikan::Application 
{
public:
	Sandbox() {
	//	PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	//	PushLayer(new ChessApp());
	}
	~Sandbox() {

	}
};

Hurikan::Application* Hurikan::CreateApplication() {
	return new Sandbox();
}