#pragma once


#ifdef _WIN32
    #include <GL/GL.h>
#else
    #include <GL/gl.h>
#endif

 GLAPI void GLAPIENTRY glGenVertexArrays( 	GLsizei n,
  	GLuint *arrays);

 GLAPI GLuint GLAPIENTRY glCreateShader( 	GLenum shaderType);

 GLAPI void GLAPIENTRY glShaderSource( 	GLuint shader,
  	GLsizei count,
  	const GLchar **string,
  	const GLint *length);


GLAPI void GLAPIENTRY glBindVertexArray( 	GLuint array);


GLAPI void GLAPIENTRY glGenBuffers( 	GLsizei n, GLuint * buffers);

GLAPI void GLAPIENTRY glBindBuffer( 	GLenum target,
  	GLuint buffer);

GLAPI void GLAPIENTRY glCompileShader( 	GLuint shader);

GLAPI void GLAPIENTRY glBufferData(  GLenum target,
  	GLsizeiptr size,
  	const void * data,
  	GLenum usage);

GLAPI void GLAPIENTRY glGetShaderiv(	GLuint shader,
 	GLenum pname,
 	GLint *params);

GLAPI void GLAPIENTRY glDeleteBuffers( 	GLsizei n,
  	const GLuint * buffers);

GLAPI void GLAPIENTRY glGetShaderInfoLog( 	GLuint shader,
  	GLsizei maxLength,
  	GLsizei *length,
  	GLchar *infoLog);

GLAPI GLuint GLAPIENTRY glCreateProgram( 	void);

GLAPI void GLAPIENTRY glBindVertexArray( 	GLuint array);

GLAPI void GLAPIENTRY glGenerateMipmap( 	GLenum target);

GLAPI void GLAPIENTRY glAttachShader( 	GLuint program,
  	GLuint shader);

GLAPI void GLAPIENTRY glLinkProgram( 	GLuint program);

GLAPI void GLAPIENTRY glDrawArraysInstanced( 	GLenum mode,
  	GLint first,
  	GLsizei count,
  	GLsizei instancecount);

GLAPI void GLAPIENTRY glUniform1f( 	GLint location,
  	GLfloat v0);
 
GLAPI void GLAPIENTRY glUniform2f( 	GLint location,
  	GLfloat v0,
  	GLfloat v1);
 
GLAPI void GLAPIENTRY glUniform3f( 	GLint location,
  	GLfloat v0,
  	GLfloat v1,
  	GLfloat v2);
 
GLAPI void GLAPIENTRY glUniform4f( 	GLint location,
  	GLfloat v0,
  	GLfloat v1,
  	GLfloat v2,
  	GLfloat v3);
 
GLAPI void GLAPIENTRY glUniform1i( 	GLint location,
  	GLint v0);
 
GLAPI void GLAPIENTRY glUniform2i( 	GLint location,
  	GLint v0,
  	GLint v1);
 
GLAPI void GLAPIENTRY glUniform3i( 	GLint location,
  	GLint v0,
  	GLint v1,
  	GLint v2);
 
GLAPI void GLAPIENTRY glUniform4i( 	GLint location,
  	GLint v0,
  	GLint v1,
  	GLint v2,
  	GLint v3);
 
GLAPI void GLAPIENTRY glUniform1ui( 	GLint location,
  	GLuint v0);
 
GLAPI void GLAPIENTRY glUniform2ui( 	GLint location,
  	GLuint v0,
  	GLuint v1);
 
GLAPI void GLAPIENTRY glUniform3ui( 	GLint location,
  	GLuint v0,
  	GLuint v1,
  	GLuint v2);
 
GLAPI void GLAPIENTRY glUniform4ui( 	GLint location,
  	GLuint v0,
  	GLuint v1,
  	GLuint v2,
  	GLuint v3);
 
GLAPI void GLAPIENTRY glUniform1fv( 	GLint location,
  	GLsizei count,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniform2fv( 	GLint location,
  	GLsizei count,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniform3fv( 	GLint location,
  	GLsizei count,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniform4fv( 	GLint location,
  	GLsizei count,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniform1iv( 	GLint location,
  	GLsizei count,
  	const GLint *value);
 
GLAPI void GLAPIENTRY glUniform2iv( 	GLint location,
  	GLsizei count,
  	const GLint *value);
 
GLAPI void GLAPIENTRY glUniform3iv( 	GLint location,
  	GLsizei count,
  	const GLint *value);
 
GLAPI void GLAPIENTRY glUniform4iv( 	GLint location,
  	GLsizei count,
  	const GLint *value);
 
GLAPI void GLAPIENTRY glUniform1uiv( 	GLint location,
  	GLsizei count,
  	const GLuint *value);
 
GLAPI void GLAPIENTRY glUniform2uiv( 	GLint location,
  	GLsizei count,
  	const GLuint *value);
 
GLAPI void GLAPIENTRY glUniform3uiv( 	GLint location,
  	GLsizei count,
  	const GLuint *value);
 
GLAPI void GLAPIENTRY glUniform4uiv( 	GLint location,
  	GLsizei count,
  	const GLuint *value);
 
GLAPI void GLAPIENTRY glUniformMatrix2fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix3fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix4fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix2x3fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix3x2fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix2x4fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix4x2fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix3x4fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);
 
GLAPI void GLAPIENTRY glUniformMatrix4x3fv( 	GLint location,
  	GLsizei count,
  	GLboolean transpose,
  	const GLfloat *value);

GLAPI void GLAPIENTRY glEnableVertexAttribArray( 	GLuint index);

GLAPI void GLAPIENTRY glDeleteProgram( 	GLuint program);

GLAPI void GLAPIENTRY glVertexAttribDivisor( 	GLuint index,
  	GLuint divisor);

GLAPI void GLAPIENTRY glDeleteVertexArrays( 	GLsizei n,
  	const GLuint *arrays);

GLAPI void GLAPIENTRY glGetProgramiv(	GLuint program,
 	GLenum pname,
 	GLint *params);

GLAPI void GLAPIENTRY glDeleteShader( 	GLuint shader);

GLAPI void GLAPIENTRY glVertexAttribPointer( 	GLuint index,
  	GLint size,
  	GLenum type,
  	GLboolean normalized,
  	GLsizei stride,
  	const void * pointer);

GLAPI void GLAPIENTRY glUseProgram( 	GLuint program);

GLAPI GLenum GLAPIENTRY glCheckFramebufferStatus( 	GLenum target);

GLAPI void GLAPIENTRY glDeleteFramebuffers( 	GLsizei n,
  	GLuint *framebuffers);

GLAPI void GLAPIENTRY glBindFramebuffer( 	GLenum target,
  	GLuint framebuffer);

GLAPI GLint GLAPIENTRY glGetUniformLocation( 	GLuint program,
  	const GLchar *name);

GLAPI void GLAPIENTRY glDrawBuffers( 	GLsizei n,
  	const GLenum *bufs);

GLAPI void GLAPIENTRY glGenFramebuffers( 	GLsizei n,
  	GLuint *ids);

GLAPI void GLAPIENTRY glFramebufferTexture2D(	GLenum target,
 	GLenum attachment,
 	GLenum textarget,
 	GLuint texture,
 	GLint level);

GLAPI void GLAPIENTRY glVertexAttribIPointer( 	GLuint index,
  	GLint size,
  	GLenum type,
  	GLsizei stride,
  	const void * pointer);

GLAPI const GLubyte * GLAPIENTRY gluErrorString(	GLenum error);

GLAPI void GLAPIENTRY glNamedBufferData( 	GLuint buffer,
  	GLsizeiptr size,
  	const void *data,
  	GLenum usage);

GLAPI void GLAPIENTRY glNamedBufferSubData( 	GLuint buffer,
  	GLintptr offset,
  	GLsizeiptr size,
  	const void *data);


