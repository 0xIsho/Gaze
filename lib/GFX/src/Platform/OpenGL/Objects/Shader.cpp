#include "GFX/Platform/OpenGL/Objects/Shader.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	Shader::Shader(Type type, std::string source)
		: Object([type] {
			return glCreateShader(type == Type::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		}())
	{
		const auto* src = source.c_str();
		glShaderSource(ID(), 1, &src, nullptr);
	}

	auto Shader::Release(GLID& id) -> void
	{
		glDeleteShader(id);
		id = 0;
	}

	auto Shader::Compile() const -> bool
	{
		glCompileShader(ID());

		return WasSuccessfullyCompiled();
	}

	auto Shader::RetrieveErrorLog(I32 nBytes) const -> std::string
	{
		if (WasSuccessfullyCompiled()) {
			return "";
		}

		auto logLength = 0;
		glGetShaderiv(ID(), GL_INFO_LOG_LENGTH, &logLength);

		if (nBytes > 0) {
			logLength = std::min(nBytes, logLength);
		}

		auto log = std::string();
		log.resize(static_cast<std::size_t>(logLength));

		glGetShaderInfoLog(ID(), logLength, nullptr, log.data());

		return log;
	}

	auto Shader::WasSuccessfullyCompiled() const -> bool
	{
		auto status = GL_FALSE;
		glGetShaderiv(ID(), GL_COMPILE_STATUS, &status);

		return status == GL_TRUE;
	}

	ShaderProgram::ShaderProgram()
		: Object([] { return glCreateProgram(); }())
	{
	}

	ShaderProgram::ShaderProgram(std::initializer_list<const Shader*> shaders)
		: ShaderProgram()
	{
		for (auto shader : shaders) {
			glAttachShader(ID(), shader->ID());
		}
	}

	auto ShaderProgram::Release(GLID& id) -> void
	{
		glDeleteProgram(id);
		id = 0;
	}

	auto ShaderProgram::Use() const -> void
	{
		glUseProgram(ID());
	}

	auto ShaderProgram::Link() const -> bool
	{
		glLinkProgram(ID());

		return WasSuccessfullyLinked();
	}

	auto ShaderProgram::RetrieveErrorLog(I32 nBytes) const -> std::string
	{
		if (WasSuccessfullyLinked()) {
			return "";
		}

		auto logLength = 0;
		glGetProgramiv(ID(), GL_INFO_LOG_LENGTH, &logLength);

		if (nBytes > 0) {
			logLength = std::min(nBytes, logLength);
		}

		auto log = std::string();
		log.resize(static_cast<std::size_t>(logLength));

		glGetProgramInfoLog(ID(), logLength, nullptr, log.data());

		return log;
	}

	auto ShaderProgram::UploadUniform1F(std::string name, const float val) -> bool
	{
		const auto location = RetreiveUniformLocation(name);
		if (location == -1) {
			return false;
		}

		glProgramUniform1f(ID(), location, val);

		return true;
	}

	auto ShaderProgram::UploadUniform3FV(std::string name, const float vec[3]) -> bool
	{
		const auto location = RetreiveUniformLocation(name);
		if (location == -1) {
			return false;
		}

		glProgramUniform3fv(ID(), location, 1, vec);

		return true;
	}

	auto ShaderProgram::UploadUniform4FV(std::string name, const float vec[4]) -> bool
	{
		const auto location = RetreiveUniformLocation(name);
		if (location == -1) {
			return false;
		}

		glProgramUniform4fv(ID(), location, 1, vec);

		return true;
	}

	auto ShaderProgram::UploadUniformMatrix4FV(std::string name, const F32 matrix[4 * 4]) -> bool
	{
		const auto location = RetreiveUniformLocation(name);
		if (location == -1) {
			return false;
		}

		glProgramUniformMatrix4fv(ID(), location, 1, false, matrix);

		return true;
	}

	auto ShaderProgram::RetreiveUniformLocation(std::string_view name) -> int
	{
		return glGetUniformLocation(ID(), name.data());
	}

	auto ShaderProgram::WasSuccessfullyLinked() const -> bool
	{
		auto status = GL_FALSE;
		glGetProgramiv(ID(), GL_LINK_STATUS, &status);

		return status == GL_TRUE;
	}
}
