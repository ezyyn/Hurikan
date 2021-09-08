#include "hupch.h"
#include "OpenGLShader.h"

#include <fstream>
#include "glm/gtc/type_ptr.hpp"

#include "glad/glad.h"

namespace Hurikan {

	static GLenum ShaderTypeFromString(const std::string& type) 
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if(type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		HU_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		HU_PROFILE_FUNCTION();
		std::string loaded_shader = ReadFile(filepath);
		auto shaderSources = PreProcess(loaded_shader);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');

		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash,count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment) 
		: m_RendererID(0), m_Name(name)
	{
		HU_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertex;
		shaderSources[GL_FRAGMENT_SHADER] = fragment;
		Compile(shaderSources);
	}

	const std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		HU_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			HU_CORE_ERROR("Could not open file {0}", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		HU_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HU_CORE_ASSERT(eol != std::string::npos, "Syntax error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HU_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification!");

			size_t nextLinePos = source.find_first_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSource[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSource;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSource)
	{
		HU_PROFILE_FUNCTION();

		GLuint program = glCreateProgram(); // -- PROGRAM

		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& key : shaderSource)
		{
			GLenum shaderType = key.first;
			const std::string& source = key.second;

			GLuint shader = glCreateShader(shaderType);
			// Create an empty vertex shader handle

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCSTR = source.c_str();
			glShaderSource(shader, 1, &sourceCSTR, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				HU_CORE_ERROR("{0}", infoLog.data());
				HU_CORE_ASSERT(false,"Shader failed to compile!");
				return;
			}
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			HU_CORE_ERROR("{0}", infoLog.data());
			HU_CORE_ASSERT("Linking process has failed to compile!", false);
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	void Hurikan::OpenGLShader::Bind() const
	{
		HU_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void Hurikan::OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	Hurikan::OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(0);
	}

/**********************************************************************************************************************/
/*******  SETTING UNIFORMS FROM CPU  **********************************************************************************/
/**********************************************************************************************************************/
	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		HU_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		HU_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		HU_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		HU_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		HU_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		HU_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

/**********************************************************************************************************************/
/*******  UPLOADING UNIFORMS TO GPU  **********************************************************************************/
/**********************************************************************************************************************/

	void OpenGLShader::UploadUniformInt(const std::string& name, int v0)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, v0);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float v0)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, v0);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, v0, v1, v2, v3);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& v)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1 && false) {
			HU_CORE_ASSERT(false, name);
			HU_CORE_ASSERT(false, "Uniform not found!");
		}
		glUniform4f(location, v.r, v.g, v.b, v.a);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, float v0, float v1)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, v0,v1);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& v)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, v.x, v.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, float v0, float v1, float v2)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, v0, v1,v2);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& v)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, v.x,v.y,v.z);
	}

}