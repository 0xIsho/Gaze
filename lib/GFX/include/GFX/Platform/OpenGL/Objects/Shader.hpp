#pragma once

#include "Object.hpp"

#include <string>
#include <string_view>
#include <initializer_list>

namespace Gaze::GFX::Platform::OpenGL::Objects {
	class Shader : public Object<Shader>
	{
	public:
		enum class Type
		{
			Vertex,
			Fragment
		};

	public:
		Shader(Type type, std::string source);
		static auto Release(GLID& id) -> void;

		[[nodiscard]]
		auto Compile() const -> bool;

		[[nodiscard]]
		auto RetrieveErrorLog(I32 nBytes) const -> std::string;

	private:
		[[nodiscard]]
		auto WasSuccessfullyCompiled() const -> bool;
	};

	class ShaderProgram : public Object<ShaderProgram>
	{
	public:
		ShaderProgram();
		ShaderProgram(std::initializer_list<const Shader*> shaders);
		static auto Release(GLID& id) -> void;

		auto Bind() const -> void;

		[[nodiscard]]
		auto Link() const -> bool;

		auto RetrieveErrorLog(I32 nBytes) const -> std::string;

		auto UploadUniform4FV(std::string name, const float vec[4]) -> bool;
		auto UploadUniformMatrix4FV(std::string name, const F32 matrix[4 * 4]) -> bool;

	private:
		[[nodiscard]]
		auto RetreiveUniformLocation(std::string_view name) -> int;
		[[nodiscard]]
		auto WasSuccessfullyLinked() const -> bool;
	};
}
