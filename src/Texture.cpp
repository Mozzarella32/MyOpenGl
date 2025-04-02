#include "Utilities.hpp"

#include "Texture.hpp"

#include "Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>


#ifdef _WIN32
#include <windows.h>
#define TRAP() DebugBreak()
#elif defined(__linux__)
#include <signal.h>
#define TRAP() raise(SIGTRAP)
#else
#error "Platform not supported for TRAP()"
#endif

const Texture::Descriptor Texture::DefaultDescriptor = {};

void Texture::InitializeTexture(void* textureBuffer) {
	GLCALL(glBindTexture(GL_TEXTURE_2D, TextureId));
	GLCALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, desc.BorderColour.data()));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.Wrap_S));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.Wrap_T));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, desc.Wrap_R));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.Min_Filter));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.Mag_Filter));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, desc.Swizzle_R));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, desc.Swizzle_G));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, desc.Swizzle_B));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, desc.Swizzle_A));

	GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, desc.Min_LOD));
	GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, desc.Max_LOD));
	GLCALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, desc.LOD_Bias));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, desc.Compare_Mode));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, desc.Compare_Function));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, desc.Depth_Stencil_Texture_Mode));

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, desc.Internal_Format, Width, Height, 0, desc.Format, desc.Type, textureBuffer));

	if (desc.Generate_Mipmaps) {
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	}

	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::Texture(const std::filesystem::path& Path, const Descriptor& desc)
	:desc(desc)
{
	GLCALL(glGenTextures(1, &TextureId));
	auto textureBuffer = stbi_load(Path.string().c_str(), &Width, &Height, &bitsPerPixel, 4);
	InitializeTexture(textureBuffer);

	if (glIsTexture(TextureId) == GL_FALSE) {
		ERRORLOG("InvalidTextureId");
		TRAP();
	}

	stbi_image_free(textureBuffer);
}


Texture::Texture(int Width, int Height, void* pixels, const Descriptor& desc)
	: Width(Width), Height(Height), desc(desc) {
	GLCALL(glGenTextures(1, &TextureId));
	InitializeTexture(pixels);
}

Texture::Texture(const wxImage& Image, const Descriptor& desc)
	:Texture(Image.GetWidth(), Image.GetHeight(),
		[&Image]() -> std::vector<unsigned char> {
			size_t Size = Image.GetWidth() * Image.GetHeight();
			if (!Image.HasAlpha()) {
				return std::vector<unsigned char>(Image.GetData(), Image.GetData() + 3 * Size);
			}

			auto Data = Image.GetData();
			auto Alpha = Image.GetAlpha();
			std::vector<unsigned char> Bytes(Size * 4, 0);
			for (size_t i = 0; i < Size; i++) {
				Bytes[4 * i + 0] = Data[3 * i + 0];
				Bytes[4 * i + 1] = Data[3 * i + 1];
				Bytes[4 * i + 2] = Data[3 * i + 2];
				Bytes[4 * i + 3] = Alpha[i];
			}
			return Bytes;
		}().data(),
			[&Image, desc = desc]() mutable {
			desc.Internal_Format = Image.HasAlpha() ? GL_RGBA : GL_RGB;
			return desc;
			}()) {}

Texture::~Texture() {
	GLCALL(glDeleteTextures(1, &TextureId));
}

wxImage Texture::ToWxImage() const {
	// Bind the texture
	GLCALL(glBindTexture(GL_TEXTURE_2D, TextureId));

	// Determine if the texture has an alpha channel
	bool hasAlpha = (desc.Internal_Format == GL_RGBA);

	// Create a buffer to hold the texture data
	size_t pixelSize = hasAlpha ? 4 : 3;
	std::vector<unsigned char> pixels(Width * Height * pixelSize);

	// Get the texture data
	GLCALL(glGetTexImage(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels.data()));

	// Unbind the texture
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	if (!hasAlpha) {
		// Allocate memory with malloc for wxImage to take ownership
		unsigned char* data = static_cast<unsigned char*>(malloc(Width * Height * 3));
		std::memcpy(data, pixels.data(), Width * Height * 3);
		return wxImage(Width, Height, data, false);
	}
	else {
		// Allocate memory with malloc for wxImage to take ownership
		unsigned char* imageData = static_cast<unsigned char*>(malloc(Width * Height * 3));
		unsigned char* alphaData = static_cast<unsigned char*>(malloc(Width * Height));

		for (int i = 0; i < Width * Height; ++i) {
			imageData[i * 3 + 0] = pixels[i * pixelSize + 0];
			imageData[i * 3 + 1] = pixels[i * pixelSize + 1];
			imageData[i * 3 + 2] = pixels[i * pixelSize + 2];
			alphaData[i] = pixels[i * pixelSize + 3];
		}
		return wxImage(Width, Height, imageData, alphaData, false);
	}
}


void Texture::Resize(int Width, int Height, void* pixels) {
	if (this->Width == Width && this->Height && pixels == nullptr)return;
	this->Width = Width;
	this->Height = Height;
	GLCALL(glBindTexture(GL_TEXTURE_2D, TextureId));

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, desc.Internal_Format, Width, Height, 0, desc.Format, desc.Type, pixels));

	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::bind(Shader* shader, const GLchar* TextureUniformName, const GLchar* TextureUniformSize, unsigned int Pos) {
	assert(Pos < 32);
	if (TextureUniformName != "")
		GLCALL(glUniform1i(shader->GetLocation(TextureUniformName), Pos));
	if (TextureUniformSize != "")
		GLCALL(glUniform2f(shader->GetLocation(TextureUniformSize), Width, Height));
	GLCALL(glActiveTexture(Pos + GL_TEXTURE0));
	GLCALL(glBindTexture(GL_TEXTURE_2D, TextureId));
}

void Texture::bind() {
	GLCALL(glBindTexture(GL_TEXTURE_2D, TextureId));
}

void Texture::unbind() {
#ifdef _DEBUG
	GLCALL(glActiveTexture(GL_TEXTURE0));
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
#endif
}

int Texture::GetWidth() const {
	return Width;
}

int Texture::GetHeight() const {
	return Height;
}

wxSize Texture::GetSize() const {
	return { Width,Height };
}

GLuint Texture::GetId() const {
	return TextureId;
}

FrameBufferObject::FrameBufferObject(const std::vector<Texture*>& textures, const std::vector<GLenum>& attatchments)
	:textures(textures), attatchments(attatchments) {
	if (textures.size() != attatchments.size()) {
		std::ofstream o("Error.log", std::ios::app);
		o << "Framebuffer error: Textures and attatchments size mismatch" << std::endl;
		TRAP();
	}
	GLCALL(glGenFramebuffers(1, &FramebufferId));
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId));

	std::vector<GLenum> ActualAttatchments;
	for (const GLenum& att : attatchments) {
		if (att == GL_DEPTH_ATTACHMENT || att == GL_STENCIL_ATTACHMENT || att == GL_DEPTH_STENCIL_ATTACHMENT) continue;
		ActualAttatchments.push_back(att);
	}

	GLCALL(glDrawBuffers(ActualAttatchments.size(), ActualAttatchments.data()));

	for (size_t i = 0; i < textures.size(); i++) {
		GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attatchments[i], GL_TEXTURE_2D, textures[i]->TextureId, 0));
	}
	auto status = GLCALL(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::ofstream o("Error.log", std::ios::app);
		switch (status) {
		case GL_FRAMEBUFFER_COMPLETE:
			// Framebuffer is complete, nothing to do here
			break;
		case GL_FRAMEBUFFER_UNDEFINED:
			// The default framebuffer does not exist
			o << "Framebuffer error: Default framebuffer does not exist" << std::endl;
			TRAP();
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			// One or more framebuffer attachments are incomplete or not attached
			o << "Framebuffer error: Incomplete attachment" << std::endl;
			TRAP();
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			// No images are attached to the framebuffer
			o << "Framebuffer error: No attachments" << std::endl;
			TRAP();
			break;
			// Add more cases for other possible error codes as needed
		default:
			// Unknown error
			o << "Framebuffer error: Unknown error" << std::endl;
			TRAP();
			break;
		}
	}
	GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBufferObject::~FrameBufferObject() {
	GLCALL(glDeleteFramebuffers(1, &FramebufferId));
}

void FrameBufferObject::bind(BindMode mode) {
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

	switch (mode) {
	case Read:
		GLCALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, FramebufferId));
		break;
	case Draw:
		GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FramebufferId));
		break;
	case ReadDraw:
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, FramebufferId));
		break;
	}

	LastBind = mode;

}

void FrameBufferObject::unbind() {

	switch (LastBind) {
	case Read:
		GLCALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
		break;
	case Draw:
		GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		break;
	case ReadDraw:
		GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		break;
	}

}

GLuint FrameBufferObject::GetId() const {
	return FramebufferId;
}

//
//Texture* FrameBufferObject::GetTexture() const {
//	return texture;
//}

std::vector<Texture*> FrameBufferObject::GetTextures() const {
	return textures;
}


std::vector<GLenum> FrameBufferObject::GetAttatchments() const {
	return attatchments;
}
