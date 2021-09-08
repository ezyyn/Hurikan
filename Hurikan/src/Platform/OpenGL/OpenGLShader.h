#pragma once

#include "Hurikan/Renderer/Shader.h"

#include "glm/glm.hpp"

typedef unsigned int GLenum;

namespace Hurikan {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment);

		virtual const std::string& GetName() const override { return m_Name; }

		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

/**********************************************************************************************************************/
/*******  SETTING UNIFORMS FROM CPU  **********************************************************************************/
/**********************************************************************************************************************/
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values,uint32_t count) override;

		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

/**********************************************************************************************************************/
/*******  UPLOADING UNIFORMS TO GPU  **********************************************************************************/
/**********************************************************************************************************************/

		void UploadUniformInt(const std::string& name, int v0);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadUniformFloat(const std::string& name, float v0);
		void UploadUniformFloat2(const std::string& name, float v0, float v1);
		void UploadUniformFloat3(const std::string& name, float v0, float v1, float v2);
		void UploadUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3);
		
		void UploadUniformFloat2(const std::string& name, const glm::vec2& v);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& v);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& v);

		/*
		void UploadUniformInt2(const std::string& name, int v0, int v1);
		void UploadUniformInt3(const std::string& name, int v0, int v1, int v2);
		void UploadUniformInt4(const std::string& name, int v0, int v1, int v2, int v3);
		*/
	private:
		const std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum,std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSource);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}

