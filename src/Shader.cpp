#include "Shader.hpp"

#include "Utilities.hpp"
#include <functional>
#include <variant>

GLuint Shader::compile(const std::string &shaderSource, GLenum type,
                       ErrorHandler err) {
  GLuint id = GLCALL(glCreateShader(type));

  const char *src = shaderSource.c_str();
  GLCALL(glShaderSource(id, 1, &src, 0));
  GLCALL(glCompileShader(id));

  int result;
  GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result != GL_TRUE) {
    int length = 0;
    GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char *message = new char[length];
    GLCALL(glGetShaderInfoLog(id, length, &length, message));

    std::string ShaderType;
    if (type == GL_VERTEX_SHADER)
      ShaderType = "Vertex Shader";
    if (type == GL_FRAGMENT_SHADER)
      ShaderType = "Fragment Shader";

    err("Shader compilation faild", "With Shadertype: " + ShaderType +
                                        "\nError: \n" + std::string(message) +
                                        "\nShader Source: \n" + shaderSource);
    delete[] message;
    return 0;
  }

  return id;
}

std::string Shader::parse(const std::filesystem::path &filename,
                          ErrorHandler err) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    err("Faild to open File", "Faild to open : " + filename.string());
    return "";
  }
  std::stringstream contentStream;
  contentStream << file.rdbuf();
  file.close();

  return contentStream.str();
}

Shader::Shader(ErrorHandler err,
               const std::filesystem::path &vertexShaderFilename,
               const std::filesystem::path &fragmentShaderFilename,
               const std::filesystem::path &geometryShaderFilename)
    : Shader(err, parse(vertexShaderFilename, err),
             parse(fragmentShaderFilename, err),
             parse(geometryShaderFilename, err)) {}

Shader::Shader(ErrorHandler err,
               const std::filesystem::path &vertexShaderFilename,
               const std::filesystem::path &fragmentShaderFilename)
    : Shader(err, parse(vertexShaderFilename, err),
             parse(fragmentShaderFilename, err)) {}

Shader::Shader(ErrorHandler err, const std::string &vertexShader,
               const std::string &fragmentShader,
               const std::string &geometryShader) {
  shaderId = GLCALL(glCreateProgram());
  GLenum error = glGetError();
  if (shaderId == 0) {
    if (shaderId == 0) {
      err("Shader Creation Error", "Failed to create shader program.");
    } else {
      err("OpenGL Error", "OpenGL error occurred: " + std::to_string(error));
    }
    return;
  }

  GLuint vs = compile(vertexShader, GL_VERTEX_SHADER, err);
  GLuint fs = compile(fragmentShader, GL_FRAGMENT_SHADER, err);
  GLuint gs = geometryShader.empty()
                  ? 0
                  : compile(geometryShader, GL_GEOMETRY_SHADER, err);

  GLCALL(glAttachShader(shaderId, vs));
  GLCALL(glAttachShader(shaderId, fs));

  if (!geometryShader.empty()) {
    GLCALL(glAttachShader(shaderId, gs));
  }

  GLCALL(glLinkProgram(shaderId));
  int result;
  GLCALL(glGetProgramiv(shaderId, GL_LINK_STATUS, &result));
  if (result != GL_TRUE) {
    int length = 0;
    GLCALL(glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &length));
    char *message = new char[length];
    GLCALL(glGetProgramInfoLog(shaderId, length, &length, message));
    err("Shader Link Error",
        "Error: " + std::string(message) + "\n Vert: " + vertexShader +
            "\n Frag: " + fragmentShader +
            (geometryShader != "" ? "\n Geo: " + geometryShader : ""));
    delete[] message;
    GLCALL(glDeleteProgram(shaderId));
    return;
  }

  GLint numUniforms = 0;
  glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &numUniforms);

  for(int i = 0; i < numUniforms; ++i) {
      char name[128];
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(shaderId, i, sizeof(name), &length, &size, &type, name);
      uniformInfo[name] = {glGetUniformLocation(shaderId, name), 0};
  }

#ifndef NDEBUG
  GLCALL(glDetachShader(shaderId, vs));
  GLCALL(glDetachShader(shaderId, fs));
  if (!geometryShader.empty()) {
    GLCALL(glDetachShader(shaderId, gs));
  }
#endif
  GLCALL(glDeleteShader(vs));
  GLCALL(glDeleteShader(fs));
  if (!geometryShader.empty()) {
    GLCALL(glDeleteShader(gs));
  }
}

Shader::~Shader() { GLCALL(glDeleteProgram(shaderId)); }

void Shader::bind() { GLCALL(glUseProgram(shaderId)); }

void Shader::unbind() {
#ifndef NDEBUG
  GLCALL(glUseProgram(0));
#endif
}

const GLuint &Shader::GetId() const { return shaderId; }

template<typename T>
concept hashable = requires (const T t) {
  { t.hash() } -> std::same_as<size_t>;
};

template<typename T>
concept applyable = requires (const T tconst, GLint location) {
  { tconst.apply(location) } -> std::same_as<void>;
};

void Shader::applyUniform(const std::string& name, const UniformData& data){
	auto it = uniformInfo.find(name);
	if (it == uniformInfo.end()) {
		ERRORLOG("Uniform not active or not existent");
		return;
	}
	auto& info = it->second;

  Visitor hasher{
    [](const hashable auto & h) {
      return h.hash();
    }
  };

  const auto newHash = std::visit(hasher, data);

  if(info.lastHash == newHash) return;

  info.lastHash = newHash;

	Visitor applyer{
	  [location = info.location](const applyable auto& a) {
	    return a.apply(location);
	  }
	};

	std::visit(applyer, data);
}

template <class T>
static inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

size_t Shader::Uniform1fData::hash() const { return std::hash<GLfloat>{}(v0); }
void Shader::Uniform1fData::apply(GLint location) const {
  GLCALL(glUniform1f(location, v0));
}

size_t Shader::Uniform2fData::hash() const {
  auto s = std::hash<GLfloat>{}(v0);
  hash_combine(s, v1);
  return s;
}
void Shader::Uniform2fData::apply(GLint location) const {
  GLCALL(glUniform2f(location, v0, v1));
}

size_t Shader::Uniform3fData::hash() const {
  auto s = std::hash<GLfloat>{}(v0);
  hash_combine(s, v1);
  hash_combine(s, v2);
  return s;
}
void Shader::Uniform3fData::apply(GLint location) const {
  GLCALL(glUniform3f(location, v0, v1, v2));
}

size_t Shader::Uniform4fData::hash() const {
  auto s = std::hash<GLfloat>{}(v0);
  hash_combine(s, v1);
  hash_combine(s, v2);
  hash_combine(s, v3);
  return s;
}
void Shader::Uniform4fData::apply(GLint location) const {
  GLCALL(glUniform4f(location, v0, v1, v2, v3));
}

size_t Shader::Uniform1iData::hash() const { return std::hash<GLint>{}(v0); }
void Shader::Uniform1iData::apply(GLint location) const {
  GLCALL(glUniform1i(location, v0));
}

size_t Shader::Uniform2iData::hash() const {
  auto s = std::hash<GLint>{}(v0);
  hash_combine(s, v1);
  return s;
}
void Shader::Uniform2iData::apply(GLint location) const {
  GLCALL(glUniform2i(location, v0, v1));
}

size_t Shader::Uniform3iData::hash() const {
  auto s = std::hash<GLint>{}(v0);
  hash_combine(s, v1);
  hash_combine(s, v2);
  return s;
}
void Shader::Uniform3iData::apply(GLint location) const {
  GLCALL(glUniform3i(location, v0, v1, v2));
}

size_t Shader::Uniform4iData::hash() const {
  auto s = std::hash<GLint>{}(v0);
  hash_combine(s, v1);
  hash_combine(s, v2);
  hash_combine(s, v3);
  return s;
}
void Shader::Uniform4iData::apply(GLint location) const {
  GLCALL(glUniform4i(location, v0, v1, v2, v3));
}

size_t Shader::Uniform1uiData::hash() const { return std::hash<GLuint>{}(v0); }
void Shader::Uniform1uiData::apply(GLint location) const {
  GLCALL(glUniform1ui(location, v0));
}

size_t Shader::Uniform2uiData::hash() const {
  auto s = std::hash<GLuint>{}(v0);
  hash_combine(s, v1);
  return s;
}
void Shader::Uniform2uiData::apply(GLint location) const {
  GLCALL(glUniform2ui(location, v0, v1));
}

size_t Shader::Uniform3uiData::hash() const {
  auto s = std::hash<GLuint>{}(v0);
  hash_combine(s, v1);
  hash_combine(s, v2);
  return s;
}
void Shader::Uniform3uiData::apply(GLint location) const {
  GLCALL(glUniform3ui(location, v0, v1, v2));
}

size_t Shader::Uniform4uiData::hash() const {
  auto s = std::hash<GLuint>{}(v0);
  hash_combine(s, v1);
  hash_combine(s, v2);
  hash_combine(s, v3);
  return s;
}
void Shader::Uniform4uiData::apply(GLint location) const {
  GLCALL(glUniform4ui(location, v0, v1, v2, v3));
}

size_t Shader::Uniform1fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform1fvData::apply(GLint location) const {
  GLCALL(glUniform1fv(location, count, value));
}

size_t Shader::Uniform2fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform2fvData::apply(GLint location) const {
  GLCALL(glUniform2fv(location, count, value));
}

size_t Shader::Uniform3fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform3fvData::apply(GLint location) const {
  GLCALL(glUniform3fv(location, count, value));
}

size_t Shader::Uniform4fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform4fvData::apply(GLint location) const {
  GLCALL(glUniform4fv(location, count, value));
}

size_t Shader::Uniform1ivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform1ivData::apply(GLint location) const {
  GLCALL(glUniform1iv(location, count, value));
}

size_t Shader::Uniform2ivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform2ivData::apply(GLint location) const {
  GLCALL(glUniform2iv(location, count, value));
}

size_t Shader::Uniform3ivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform3ivData::apply(GLint location) const {
  GLCALL(glUniform3iv(location, count, value));
}

size_t Shader::Uniform4ivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform4ivData::apply(GLint location) const {
  GLCALL(glUniform4iv(location, count, value));
}

size_t Shader::Uniform1uivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform1uivData::apply(GLint location) const {
  GLCALL(glUniform1uiv(location, count, value));
}

size_t Shader::Uniform2uivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform2uivData::apply(GLint location) const {
  GLCALL(glUniform2uiv(location, count, value));
}

size_t Shader::Uniform3uivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform3uivData::apply(GLint location) const {
  GLCALL(glUniform3uiv(location, count, value));
}

size_t Shader::Uniform4uivData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, value);
  return s;
}
void Shader::Uniform4uivData::apply(GLint location) const {
  GLCALL(glUniform4uiv(location, count, value));
}

size_t Shader::UniformMatrix2fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix2fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix2fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix3fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix3fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix3fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix4fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix4fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix4fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix2x3fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix2x3fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix2x3fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix3x2fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix3x2fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix3x2fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix2x4fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix2x4fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix2x4fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix4x2fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix4x2fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix4x2fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix3x4fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s, value);
  return s;
}
void Shader::UniformMatrix3x4fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix3x4fv(location, count, transpose, value));
}

size_t Shader::UniformMatrix4x3fvData::hash() const {
  auto s = std::hash<GLsizei>{}(count);
  hash_combine(s, transpose);
  hash_combine(s,value);
  return s;
}
void Shader::UniformMatrix4x3fvData::apply(GLint location) const {
  GLCALL(glUniformMatrix4x3fv(location, count, transpose, value));
}
