#pragma once

#include "Object.hpp"

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
		Shader(Type type, std::string_view source)       noexcept;
		static auto Release(GLID& id)                    noexcept -> void;
		[[nodiscard]]
		auto Compile()                             const noexcept -> bool;
		[[nodiscard]]
		auto RetrieveErrorLog(I32 nBytes)          const noexcept -> std::string;

	private:
		[[nodiscard]]
		auto WasSuccessfullyCompiled()             const noexcept -> bool;
	};

	class ShaderProgram : public Object<ShaderProgram>
	{
	public:
		ShaderProgram()                                                        noexcept;
		ShaderProgram(std::initializer_list<const Shader*> shaders)            noexcept;
		static auto Release(GLID& id)                                          noexcept -> void;

		auto Use()                                                       const noexcept -> void;

		[[nodiscard]]
		auto Link()                                                      const noexcept -> bool;

		auto RetrieveErrorLog(I32 nBytes)                                const          -> std::string;

		auto UploadUniform1F(std::string name, const float val)                noexcept -> bool;
		auto UploadUniform3FV(std::string name, const float vec[3])            noexcept -> bool;
		auto UploadUniform4FV(std::string name, const float vec[4])            noexcept -> bool;
		auto UploadUniformMatrix4FV(std::string name, const F32 matrix[4 * 4]) noexcept -> bool;

	private:
		[[nodiscard]]
		auto RetreiveUniformLocation(std::string_view name)                    noexcept -> int;
		[[nodiscard]]
		auto WasSuccessfullyLinked()                                     const noexcept -> bool;
	};
}
