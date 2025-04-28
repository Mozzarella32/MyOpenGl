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

GLAPI void GLAPIENTRY glUniform1i( 	GLint location,
  	GLint v0);

GLAPI void GLAPIENTRY glUniform2f( 	GLint location,
  	GLfloat v0,
  	GLfloat v1);

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

