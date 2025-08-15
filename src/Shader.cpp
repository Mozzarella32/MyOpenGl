#include "Shader.hpp"

#include "Utilities.hpp"

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

GLint Shader::GetLocation(const std::string &name) {
  auto it = Locations.find(name);
  if (it == end(Locations)) {
    auto Location = GLCALL(glGetUniformLocation(shaderId, name.c_str()));
    if (Location == -1) {
      ERRORLOG("getUniformLocation returned -1");
    }
    Locations[name] = Location;
    return Locations[name];
  } else
    return it->second;
}
