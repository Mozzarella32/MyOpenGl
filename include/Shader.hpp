#pragma once

#include "pch.hpp"
#include <cstddef>
#include <functional>
#include <variant>

class Shader {
public:
	//Arguments: Where the Error occured, the msg;
	using ErrorHandler = std::function<void(std::string, std::string)>;

private:
	GLuint shaderId;

	struct UniformInfo {
		GLint location;
		size_t lastHash;
	};

	std::unordered_map<std::string, UniformInfo> uniformInfo;

	GLuint compile(const std::string& shaderSource, GLenum type, ErrorHandler err);

	std::string parse(const std::filesystem::path& filename, ErrorHandler err);

public:

	Shader(ErrorHandler err, const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename, const std::filesystem::path& geometryShaderFilename);

	Shader(ErrorHandler err, const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename);

	Shader(ErrorHandler err, const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader = {});

	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) = delete;

	virtual ~Shader();

	void bind();
	void unbind();

	const GLuint& GetId() const;

	struct Uniform1fData {
	    GLfloat v0;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform2fData {
	    GLfloat v0, v1;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform3fData {
	    GLfloat v0, v1, v2;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform4fData {
	    GLfloat v0, v1, v2, v3;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform1iData {
	    GLint v0;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform2iData {
	    GLint v0, v1;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform3iData {
	    GLint v0, v1, v2;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform4iData {
	    GLint v0, v1, v2, v3;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform1uiData {
	    GLuint v0;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform2uiData {
	    GLuint v0, v1;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform3uiData {
	    GLuint v0, v1, v2;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform4uiData {
	    GLuint v0, v1, v2, v3;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform1fvData {
	    GLsizei count;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform2fvData {
	    GLsizei count;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform3fvData {
	    GLsizei count;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform4fvData {
	    GLsizei count;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform1ivData {
	    GLsizei count;
	    const GLint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform2ivData {
	    GLsizei count;
	    const GLint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform3ivData {
	    GLsizei count;
	    const GLint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform4ivData {
	    GLsizei count;
	    const GLint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform1uivData {
	    GLsizei count;
	    const GLuint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform2uivData {
	    GLsizei count;
	    const GLuint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform3uivData {
	    GLsizei count;
	    const GLuint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct Uniform4uivData {
	    GLsizei count;
	    const GLuint* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix2fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix3fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix4fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix2x3fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix3x2fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix2x4fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix4x2fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix3x4fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	struct UniformMatrix4x3fvData {
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    size_t hash() const;
	    void apply(GLint location) const;
	};
	using UniformData = std::variant<
        Uniform1fData, Uniform2fData, Uniform3fData, Uniform4fData,
        Uniform1iData, Uniform2iData, Uniform3iData, Uniform4iData,
        Uniform1uiData, Uniform2uiData, Uniform3uiData, Uniform4uiData,
        Uniform1fvData, Uniform2fvData, Uniform3fvData, Uniform4fvData,
        Uniform1ivData, Uniform2ivData, Uniform3ivData, Uniform4ivData,
        Uniform1uivData, Uniform2uivData, Uniform3uivData, Uniform4uivData,
        UniformMatrix2fvData, UniformMatrix3fvData, UniformMatrix4fvData,
        UniformMatrix2x3fvData, UniformMatrix3x2fvData, UniformMatrix2x4fvData,
        UniformMatrix4x2fvData, UniformMatrix3x4fvData, UniformMatrix4x3fvData>;

		void applyUniform(const std::string& name, const UniformData& data);
};
