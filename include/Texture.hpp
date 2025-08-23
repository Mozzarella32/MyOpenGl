#pragma once

#include "pch.hpp"

#include "Shader.hpp"

class FrameBufferObject;
class wxImage;

class Texture {
public:

	struct Descriptor {
		std::array<float, 4> BorderColour = { 0.0f, 0.0f, 0.0f, 0.0f };

		enum Wrap_Type {
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			REPEAT = GL_REPEAT,
			MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
		};

		enum Filter_Type {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		};

		Wrap_Type Wrap_S = CLAMP_TO_EDGE;
		Wrap_Type Wrap_T = CLAMP_TO_EDGE;
		Wrap_Type Wrap_R = REPEAT;

		Filter_Type Min_Filter = NEAREST;
		Filter_Type Mag_Filter = NEAREST;
		bool Generate_Mipmaps = false;

		GLint Swizzle_R = GL_RED;
		GLint Swizzle_G = GL_GREEN;
		GLint Swizzle_B = GL_BLUE;
		GLint Swizzle_A = GL_ALPHA;

		GLenum Format = GL_RGBA;
		GLenum Internal_Format = GL_RGBA;

		GLenum Type = GL_UNSIGNED_BYTE;

		GLfloat Min_LOD = -1000.0f;
		GLfloat Max_LOD = 1000.0f;
		GLfloat LOD_Bias = 0.0f;

		GLenum Compare_Mode = GL_NONE;
		GLenum Compare_Function = GL_LEQUAL;

		GLenum Depth_Stencil_Texture_Mode = GL_DEPTH_COMPONENT;
	};

	GLuint TextureId = GLuint(-1);

	const Descriptor desc;

private:

	void InitializeTexture(void* textureBuffer);

	static const Descriptor DefaultDescriptor;
public:

	Texture(const std::filesystem::path& Path, const Descriptor& desc = DefaultDescriptor);
	Texture(int Width, int Height, void* pixels = nullptr, const Descriptor& desc = DefaultDescriptor);
	// Texture(const wxImage& Image, const Descriptor& desc = DefaultDescriptor);

	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) = delete;

	~Texture();

	// wxImage ToWxImage() const;

	void Resize(int Width, int Height, void* pixels = nullptr);

	void bind(Shader* shader, const GLchar* TextureUniformName, const GLchar* TextureUniformSize, unsigned int Pos);
	void bind();

	void unbind();
private:

	int Width = 0;
	int Height = 0;
	int bitsPerPixel = 0;
public:
	int GetWidth() const;
	int GetHeight() const;
	// wxSize GetSize() const;

	GLuint GetId() const;
public:
	friend FrameBufferObject;
};

class FrameBufferObject {

public:

	enum BindMode {
		Read,
		Draw,
		ReadDraw
	};


private:

	GLuint FramebufferId = 0;

	BindMode LastBind = ReadDraw;

	std::vector<Texture*> textures;
	std::vector<GLenum> attatchments;
public:
	FrameBufferObject(const std::vector<Texture*>& textures, const std::vector<GLenum>& attatchments = { GL_COLOR_ATTACHMENT0 });

	FrameBufferObject(const FrameBufferObject&) = delete;
	FrameBufferObject(FrameBufferObject&&) = delete;
	FrameBufferObject& operator=(const FrameBufferObject&) = delete;
	FrameBufferObject& operator=(FrameBufferObject&&) = delete;

	~FrameBufferObject();

	void bind(BindMode mode = ReadDraw);
	void unbind();

	GLuint GetId() const;

	//Has been Removed
	//Texture* GetTexture() const;

	std::vector<Texture*> GetTextures() const;

	std::vector<GLenum> GetAttatchments() const;
};
