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

	static GLuint compile(const std::string& shaderSource, GLenum type, ErrorHandler err);

	static std::string parse(const std::filesystem::path& filename, ErrorHandler err);

public:

	Shader(ErrorHandler err, const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename, const std::filesystem::path& geometryShaderFilename);

	Shader(ErrorHandler err, const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename);

	Shader(ErrorHandler err, const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader = {});

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	virtual ~Shader();

	void bind() const;
	void unbind() const;

	const GLuint& GetId() const;

	GLint uniformLocation(const std::string& name) const;

	struct Data1f {
	    GLfloat v0;
	    void apply(GLint location) const;
	};
	struct Data2f {
	    GLfloat v0, v1;
	    void apply(GLint location) const;
	};
	struct Data3f { GLfloat v0, v1, v2;
	    void apply(GLint location) const;
	};
	struct Data4f {
	    GLfloat v0, v1, v2, v3;
	    void apply(GLint location) const;
	};
	struct Data1i {
	    GLint v0;
	    void apply(GLint location) const;
	};
	struct Data2i {
	    GLint v0, v1;
	    void apply(GLint location) const;
	};
	struct Data3i {
	    GLint v0, v1, v2;
	    void apply(GLint location) const;
	};
	struct Data4i {
	    GLint v0, v1, v2, v3;
	    void apply(GLint location) const;
	};
	struct Data1ui {
	    GLuint v0;
	    void apply(GLint location) const;
	};
	struct Data2ui {
	    GLuint v0, v1;
	    void apply(GLint location) const;
	};
	struct Data3ui {
	    GLuint v0, v1, v2;
	    void apply(GLint location) const;
	};
	struct Data4ui {
	    GLuint v0, v1, v2, v3;
	    void apply(GLint location) const;
	};
	struct Data1fv {
			constexpr const static size_t elements = 1;
	    GLsizei count;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct Data2fv {
			constexpr const static size_t elements = 2;
	    GLsizei count;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct Data3fv {
			constexpr const static size_t elements = 3;
	    GLsizei count;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct Data4fv {
			constexpr const static size_t elements = 4;
	    GLsizei count;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct Data1iv {
			constexpr const static size_t elements = 1;
	    GLsizei count;
	    const GLint* value;
	    void apply(GLint location) const;
	};
	struct Data2iv {
			constexpr const static size_t elements = 2;
	    GLsizei count;
	    const GLint* value;
	    void apply(GLint location) const;
	};
	struct Data3iv {
			constexpr const static size_t elements = 3;
	    GLsizei count;
	    const GLint* value;
	    void apply(GLint location) const;
	};
	struct Data4iv {
			constexpr const static size_t elements = 4;
	    GLsizei count;
	    const GLint* value;
	    void apply(GLint location) const;
	};
	struct Data1uiv {
			constexpr const static size_t elements = 1;
	    GLsizei count;
	    const GLuint* value;
	    void apply(GLint location) const;
	};
	struct Data2uiv {
			constexpr const static size_t elements = 2;
	    GLsizei count;
	    const GLuint* value;
	    void apply(GLint location) const;
	};
	struct Data3uiv {
			constexpr const static size_t elements = 3;
	    GLsizei count;
	    const GLuint* value;
	    void apply(GLint location) const;
	};
	struct Data4uiv {
			constexpr const static size_t elements = 4;
	    GLsizei count;
	    const GLuint* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix2fv {
			constexpr const static size_t elements = 2 * 2;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix3fv {
			constexpr const static size_t elements = 3 * 3;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix4fv {
			constexpr const static size_t elements = 4 * 4;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix2x3fv {
			constexpr const static size_t elements = 2 * 3;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix3x2fv {
			constexpr const static size_t elements = 3 * 2;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix2x4fv {
			constexpr const static size_t elements = 2 * 4;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix4x2fv {
			constexpr const static size_t elements = 4 * 2;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix3x4fv {
			constexpr const static size_t elements = 3 * 4;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	struct DataMatrix4x3fv {
			constexpr const static size_t elements = 4 * 3;
	    GLsizei count;
	    GLboolean transpose;
	    const GLfloat* value;
	    void apply(GLint location) const;
	};
	using UniformData = std::variant<
        Data1f, Data2f, Data3f, Data4f,
        Data1i, Data2i, Data3i, Data4i,
        Data1ui, Data2ui, Data3ui, Data4ui,
        Data1fv, Data2fv, Data3fv, Data4fv,
        Data1iv, Data2iv, Data3iv, Data4iv,
        Data1uiv, Data2uiv, Data3uiv, Data4uiv,
        DataMatrix2fv, DataMatrix3fv, DataMatrix4fv,
        DataMatrix2x3fv, DataMatrix3x2fv, DataMatrix2x4fv,
        DataMatrix4x2fv, DataMatrix3x4fv, DataMatrix4x3fv
        >;

		void apply(const std::string& name, const UniformData& data);
};
