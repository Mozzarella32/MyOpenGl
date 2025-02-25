#pragma once

#include "pch.hpp"

class Shader {
public:
	//Arguments: Where the Error occured, the msg;
	using ErrorHandler = std::function<void(std::string, std::string)>;

private:
	GLuint shaderId;

	std::unordered_map< std::string, GLint> Locations;

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

	GLint GetLocation(const std::string&);
};
//
//class ShaderBinder {
//	Shader& shader;
//	bool Unbound = false;
//public:
//	ShaderBinder(wxGLContext* Context, Shader& shader, wxGLCanvas* Canvas);
//
//	void unbind();
//
//	~ShaderBinder();
//};