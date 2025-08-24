#include "Shader.hpp"

#include "Utilities.hpp"
#include <functional>
#include <variant>
#include <ranges>

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

template <class T>
static inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template<typename T>
concept applyable = requires (const T tconst, GLint location) {
  { tconst.apply(location) } -> std::same_as<void>;
};

void Shader::apply(const std::string& name, const UniformData& data){
	auto it = uniformInfo.find(name);
	if (it == uniformInfo.end()) {
		ERRORLOG("Uniform not active or not existent");
		return;
	}
	auto& info = it->second;

  Visitor hasher{
    []<typename T>(const T& t) {
      constexpr std::size_t size = sizeof(T) / 4;
      const auto& arr = std::bit_cast<std::array<std::uint32_t,size>>(t);
      std::size_t s = 0;
      for(const auto& elem : arr) {
        hash_combine(s, elem);
      }
      return s;
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

void Shader::Data1f::apply(GLint location) const {
  GLCALL(glUniform1f(location, v0));
}

void Shader::Data2f::apply(GLint location) const {
  GLCALL(glUniform2f(location, v0, v1));
}

void Shader::Data3f::apply(GLint location) const {
  GLCALL(glUniform3f(location, v0, v1, v2));
}

void Shader::Data4f::apply(GLint location) const {
  GLCALL(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::Data1i::apply(GLint location) const {
  GLCALL(glUniform1i(location, v0));
}

void Shader::Data2i::apply(GLint location) const {
  GLCALL(glUniform2i(location, v0, v1));
}

void Shader::Data3i::apply(GLint location) const {
  GLCALL(glUniform3i(location, v0, v1, v2));
}

void Shader::Data4i::apply(GLint location) const {
  GLCALL(glUniform4i(location, v0, v1, v2, v3));
}

void Shader::Data1ui::apply(GLint location) const {
  GLCALL(glUniform1ui(location, v0));
}

void Shader::Data2ui::apply(GLint location) const {
  GLCALL(glUniform2ui(location, v0, v1));
}

void Shader::Data3ui::apply(GLint location) const {
  GLCALL(glUniform3ui(location, v0, v1, v2));
}

void Shader::Data4ui::apply(GLint location) const {
  GLCALL(glUniform4ui(location, v0, v1, v2, v3));
}

// void Shader::Uniform1fvData::apply(GLint location) const {
//   GLCALL(glUniform1fv(location, count, value));
// }

// void Shader::Uniform2fvData::apply(GLint location) const {
//   GLCALL(glUniform2fv(location, count, value));
// }

// void Shader::Uniform3fvData::apply(GLint location) const {
//   GLCALL(glUniform3fv(location, count, value));
// }

// void Shader::Uniform4fvData::apply(GLint location) const {
//   GLCALL(glUniform4fv(location, count, value));
// }

// void Shader::Uniform1ivData::apply(GLint location) const {
//   GLCALL(glUniform1iv(location, count, value));
// }

// void Shader::Uniform2ivData::apply(GLint location) const {
//   GLCALL(glUniform2iv(location, count, value));
// }

// void Shader::Uniform3ivData::apply(GLint location) const {
//   GLCALL(glUniform3iv(location, count, value));
// }

// void Shader::Uniform4ivData::apply(GLint location) const {
//   GLCALL(glUniform4iv(location, count, value));
// }

// void Shader::Uniform1uivData::apply(GLint location) const {
//   GLCALL(glUniform1uiv(location, count, value));
// }

// void Shader::Uniform2uivData::apply(GLint location) const {
//   GLCALL(glUniform2uiv(location, count, value));
// }

// void Shader::Uniform3uivData::apply(GLint location) const {
//   GLCALL(glUniform3uiv(location, count, value));
// }

// void Shader::Uniform4uivData::apply(GLint location) const {
//   GLCALL(glUniform4uiv(location, count, value));
// }

// void Shader::UniformMatrix2fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix2fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix3fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix3fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix4fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix4fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix2x3fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix2x3fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix3x2fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix3x2fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix2x4fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix2x4fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix4x2fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix4x2fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix3x4fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix3x4fv(location, count, transpose, value));
// }

// void Shader::UniformMatrix4x3fvData::apply(GLint location) const {
//   GLCALL(glUniformMatrix4x3fv(location, count, transpose, value));
// }
