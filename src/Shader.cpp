#include "Shader.hpp"

#include "Utilities.hpp"
#include <functional>
#include <type_traits>
#include <utility>
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

Shader& Shader::operator=(Shader&& other) {
  shaderId = std::exchange(other.shaderId, 0);
  uniformInfo = std::exchange(other.uniformInfo, {});
  return *this;
}

Shader::~Shader() {
  if(shaderId != 0) {
    GLCALL(glDeleteProgram(shaderId));
  }
}

void Shader::bind() const { GLCALL(glUseProgram(shaderId)); }

void Shader::unbind() const {
#ifndef NDEBUG
  GLCALL(glUseProgram(0));
#endif
}

const GLuint &Shader::GetId() const { return shaderId; }

GLint Shader::uniformLocation(const std::string& name) const {
  auto it = uniformInfo.find(name);
  if(it == uniformInfo.end()) {
    return -1;
  }
  return it->second.location;
}

template <class T>
static inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template<typename T>
concept applyable = requires (const T tconst, GLint location) {
  { tconst.apply(location) } -> std::same_as<void>;
};

template<typename T>
concept vData = requires (T t) {
  {T::elements} -> std::same_as<const size_t&>;
  {t.count} -> std::same_as<GLsizei&>;
  typename std::remove_pointer_t<decltype(t.value)>;
  requires std::is_pointer_v<decltype(t.value)>;
  requires std::is_const_v<std::remove_pointer_t<decltype(t.value)>>;
};

template<typename T>
concept vMatrix = requires (T t) {
  requires vData<T>;
  {t.transpose} -> std::same_as<GLboolean&>;
};

void Shader::apply(const std::string& name, const UniformData& data) {
	auto it = uniformInfo.find(name);
	if (it == uniformInfo.end()) {
		ERRORLOG("Uniform not active or not existent");
		return;
	}
	auto& info = it->second;

  Visitor hasher{
    []<typename T>(const T& t) requires (!vData<std::remove_cvref_t<decltype(t)>>){
      constexpr std::size_t size = sizeof(T) / 4;
      const auto& arr = std::bit_cast<std::array<std::uint32_t,size>>(t);
      std::size_t s = 0;
      for(const auto& elem : arr) {
        hash_combine(s, elem);
      }
      return s;
    },
    [](const vData auto& t) {
      using T_t = std::remove_cvref_t<decltype(t)>;
      using Elem_t = std::remove_const_t<std::remove_pointer_t<decltype(t.value)>>;
      std::size_t s = 0;
      hash_combine(s, t.count);
      if constexpr (vMatrix<T_t>){
        hash_combine(s, t.transpose);
      }
      for (size_t i = 0; i < T_t::elements * t.count; ++i) {
          hash_combine(s, std::hash<Elem_t>{}(t.value[i]));
      }
      return s;
    },
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

void Shader::Data1fv::apply(GLint location) const {
  GLCALL(glUniform1fv(location, count, value));
}

void Shader::Data2fv::apply(GLint location) const {
  GLCALL(glUniform2fv(location, count, value));
}

void Shader::Data3fv::apply(GLint location) const {
  GLCALL(glUniform3fv(location, count, value));
}

void Shader::Data4fv::apply(GLint location) const {
  GLCALL(glUniform4fv(location, count, value));
}

void Shader::Data1iv::apply(GLint location) const {
  GLCALL(glUniform1iv(location, count, value));
}

void Shader::Data2iv::apply(GLint location) const {
  GLCALL(glUniform2iv(location, count, value));
}

void Shader::Data3iv::apply(GLint location) const {
  GLCALL(glUniform3iv(location, count, value));
}

void Shader::Data4iv::apply(GLint location) const {
  GLCALL(glUniform4iv(location, count, value));
}

void Shader::Data1uiv::apply(GLint location) const {
  GLCALL(glUniform1uiv(location, count, value));
}

void Shader::Data2uiv::apply(GLint location) const {
  GLCALL(glUniform2uiv(location, count, value));
}

void Shader::Data3uiv::apply(GLint location) const {
  GLCALL(glUniform3uiv(location, count, value));
}

void Shader::Data4uiv::apply(GLint location) const {
  GLCALL(glUniform4uiv(location, count, value));
}

void Shader::DataMatrix2fv::apply(GLint location) const {
  GLCALL(glUniformMatrix2fv(location, count, transpose, value));
}

void Shader::DataMatrix3fv::apply(GLint location) const {
  GLCALL(glUniformMatrix3fv(location, count, transpose, value));
}

void Shader::DataMatrix4fv::apply(GLint location) const {
  GLCALL(glUniformMatrix4fv(location, count, transpose, value));
}

void Shader::DataMatrix2x3fv::apply(GLint location) const {
  GLCALL(glUniformMatrix2x3fv(location, count, transpose, value));
}

void Shader::DataMatrix3x2fv::apply(GLint location) const {
  GLCALL(glUniformMatrix3x2fv(location, count, transpose, value));
}

void Shader::DataMatrix2x4fv::apply(GLint location) const {
  GLCALL(glUniformMatrix2x4fv(location, count, transpose, value));
}

void Shader::DataMatrix4x2fv::apply(GLint location) const {
  GLCALL(glUniformMatrix4x2fv(location, count, transpose, value));
}

void Shader::DataMatrix3x4fv::apply(GLint location) const {
  GLCALL(glUniformMatrix3x4fv(location, count, transpose, value));
}

void Shader::DataMatrix4x3fv::apply(GLint location) const {
  GLCALL(glUniformMatrix4x3fv(location, count, transpose, value));
}

static_assert(vData<Shader::Data1fv>);
static_assert(vData<Shader::Data2fv>);
static_assert(vData<Shader::Data3fv>);
static_assert(vData<Shader::Data4fv>);
static_assert(vData<Shader::Data1iv>);
static_assert(vData<Shader::Data2iv>);
static_assert(vData<Shader::Data3iv>);
static_assert(vData<Shader::Data4iv>);
static_assert(vData<Shader::Data1uiv>);
static_assert(vData<Shader::Data2uiv>);
static_assert(vData<Shader::Data3uiv>);
static_assert(vData<Shader::Data4uiv>);

static_assert(vMatrix<Shader::DataMatrix2fv>);
static_assert(vMatrix<Shader::DataMatrix2fv>);
static_assert(vMatrix<Shader::DataMatrix3fv>);
static_assert(vMatrix<Shader::DataMatrix4fv>);
static_assert(vMatrix<Shader::DataMatrix2x3fv>);
static_assert(vMatrix<Shader::DataMatrix3x2fv>);
static_assert(vMatrix<Shader::DataMatrix2x4fv>);
static_assert(vMatrix<Shader::DataMatrix4x2fv>);
static_assert(vMatrix<Shader::DataMatrix3x4fv>);
static_assert(vMatrix<Shader::DataMatrix4x3fv>);

