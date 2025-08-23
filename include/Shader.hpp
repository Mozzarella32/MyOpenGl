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

private:
	
	template <class T>
	static inline void hash_combine(std::size_t& seed, const T& v)
	{
	    std::hash<T> hasher;
	    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	}

public:

	GLint GetLocation(const std::string& name);

    struct Uniform1fData {
      GLfloat v0;
			size_t hash() const {
				auto s = std::hash<GLfloat>{}(v0);
				return s;
			}
      void apply(GLint location) const {
				glUniform1f(location, v0);
      }
    };
    struct Uniform2fData {
      GLfloat v0;
      GLfloat v1;
			size_t hash() const {
				auto s = std::hash<GLfloat>{}(v0);
				hash_combine(s, v1);				
				return s;
			}
      void apply(GLint location) const {
				glUniform2f(location, v0, v1);
      }
    };
    struct Uniform3fData {
      GLfloat v0;
      GLfloat v1;
      GLfloat v2;
			size_t hash() const {
				auto s = std::hash<GLfloat>{}(v0);
				hash_combine(s, v1);				
				hash_combine(s, v2);				
				return s;
			}
      void apply(GLint location) const {
				glUniform3f(location, v0, v1, v2);
      }
    };
    struct Uniform4fData {
      GLfloat v0;
      GLfloat v1;
      GLfloat v2;
      GLfloat v3;
			size_t hash() const {
				auto s = std::hash<GLfloat>{}(v0);
				hash_combine(s, v1);				
				hash_combine(s, v2);				
				hash_combine(s, v3);				
				return s;
			}
      void apply(GLint location) const {
				glUniform4f(location, v0, v1, v2, v3);
      }
    };
    struct Uniform1iData {
      GLint v0;
			size_t hash() const {
				auto s = std::hash<GLint>{}(v0);
				return s;
			}
      void apply(GLint location) const {
				glUniform1i(location, v0);
      }
    };
    struct Uniform2iData {
      GLint v0;
      GLint v1;
			size_t hash() const {
				auto s = std::hash<GLint>{}(v0);
				hash_combine(s, v1);				
				return s;
			}
      void apply(GLint location) const {
				glUniform2i(location, v0, v1);
      }
    }; 
    struct Uniform3iData {
      GLint v0;
      GLint v1;
      GLint v2;
			size_t hash() const {
				auto s = std::hash<GLint>{}(v0);
				hash_combine(s, v1);				
				hash_combine(s, v2);				
				return s;
			}
      void apply(GLint location) const {
				glUniform3i(location, v0, v1, v2);
      }
    };
    struct Uniform4iData {
      GLint v0;
      GLint v1;
      GLint v2;
      GLint v3;
			size_t hash() const {
				auto s = std::hash<GLint>{}(v0);
				hash_combine(s, v1);				
				hash_combine(s, v2);				
				hash_combine(s, v3);				
				return s;
			}
      void apply(GLint location) const {
				glUniform4i(location, v0, v1, v2, v3);
      }
    };
    struct Uniform1uiData {
      GLuint v0;
			size_t hash() const {
				auto s = std::hash<GLuint>{}(v0);
				return s;
			}
      void apply(GLint location) const {
				glUniform1ui(location, v0);
      }
    };
    struct Uniform2uiData {
      GLuint v0;
      GLuint v1;
			size_t hash() const {
				auto s = std::hash<GLuint>{}(v0);
				hash_combine(s, v1);
				return s;
			}
      void apply(GLint location) const {
				glUniform2ui(location, v0, v1);
      }
    };
    struct Uniform3uiData {
      GLuint v0;
      GLuint v1;
      GLuint v2;
			size_t hash() const {
				auto s = std::hash<GLuint>{}(v0);
				hash_combine(s, v1);
				hash_combine(s, v2);
				return s;
			}
      void apply(GLint location) const {
				glUniform3ui(location, v0, v1, v2);
      }
    };
    struct Uniform4uiData {
      GLuint v0;
      GLuint v1;
      GLuint v2;
      GLuint v3;
			size_t hash() const {
				auto s = std::hash<GLuint>{}(v0);
				hash_combine(s, v1);
				hash_combine(s, v2);
				hash_combine(s, v3);
				return s;
			}
      void apply(GLint location) const {
				glUniform4ui(location, v0, v1, v2, v3);
      }
    };
    struct Uniform1fvData {
      GLsizei count;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform1fv(location, count, value);
      }
    };
    struct Uniform2fvData {
      GLsizei count;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform2fv(location, count, value);
      }
    };
    struct Uniform3fvData {
      GLsizei count;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform3fv(location, count, value);
      }
    };
    struct Uniform4fvData {
      GLsizei count;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform4fv(location, count, value);
      }
    };
    struct Uniform1ivData {
      GLsizei count;
      const GLint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform1iv(location, count, value);
      }
    };
    struct Uniform2ivData {
      GLsizei count;
      const GLint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform2iv(location, count, value);
      }
    };
    struct Uniform3ivData {
      GLsizei count;
      const GLint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform3iv(location, count, value);
      }
    };
    struct Uniform4ivData {
      GLsizei count;
      const GLint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform4iv(location, count, value);
      }
    };
    struct Uniform1uivData {
      GLsizei count;
      const GLuint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform1uiv(location, count, value);
      }
    };
    struct Uniform2uivData {
      GLsizei count;
      const GLuint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform2uiv(location, count, value);
      }
    };
    struct Uniform3uivData {
      GLsizei count;
      const GLuint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform3uiv(location, count, value);
      }
    };
    struct Uniform4uivData {
      GLsizei count;
      const GLuint *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniform4uiv(location, count, value);
      }
    };
    struct UniformMatrix2fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix2fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix3fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>{}(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix3fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix4fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>()(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix4fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix2x3fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>()(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix2x3fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix3x2fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>()(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix3x2fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix2x4fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>()(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix2x4fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix4x2fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>()(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix4x2fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix3x4fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>()(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix3x4fv(location, count, transpose, value);
      }
    };
    struct UniformMatrix4x3fvData {
      GLsizei count;
      GLboolean transpose;
      const GLfloat *value;
			size_t hash() const {
				auto s = std::hash<GLsizei>()(count);
				hash_combine(s, transpose);
				hash_combine(s, value);
				return s;
			}
      void apply(GLint location) const {
				glUniformMatrix4x3fv(location, count, transpose, value);
      }
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
