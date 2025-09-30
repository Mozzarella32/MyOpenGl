#include "VertexBuffer.hpp"


PixelBufferObject::PixelBufferObject(GLenum Usage, bool FromFBOtoPBO, size_t Bytes) {
	GLCALL(glGenBuffers(1, &PBO));
	BufferType = FromFBOtoPBO ? GL_PIXEL_PACK_BUFFER : GL_PIXEL_UNPACK_BUFFER;
	GLCALL(glBindBuffer(BufferType, PBO));
	GLCALL(glBufferStorage(BufferType, Bytes, 0, Usage));
	GLCALL(glBindBuffer(BufferType, 0));
}

void PixelBufferObject::bind() {
	GLCALL(glBindBuffer(BufferType, PBO));
}

void PixelBufferObject::unbind() {
	GLCALL(glBindBuffer(BufferType, 0));
}

VertexArrayObject::VertexArrayObject(std::vector<VertexBufferObjectDescriptor> InitialBufferDescriptors) :BufferDescriptors(InitialBufferDescriptors) {
	GLCALL(glGenVertexArrays(1, &VAO));
	GLCALL(glBindVertexArray(VAO));

	GLuint AttributePosition = 0;

	for (auto& BufferDescriptor : BufferDescriptors) {
		GLCALL(glGenBuffers(1, &BufferDescriptor.VBO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, BufferDescriptor.VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, 0, 0, BufferDescriptor.Usage));

		BufferDescriptor.PrepareVBO(AttributePosition);

#ifndef NDEBUG
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
#endif
	}
#ifndef NDEBUG
	GLCALL(glBindVertexArray(0));
#endif
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& Other) noexcept
	:VAO(std::move(Other.VAO)), BufferDescriptors(std::move(Other.BufferDescriptors))  {
	Other.VAO = 0;
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& Other) noexcept {
	VAO = std::move(Other.VAO);
	BufferDescriptors = std::move(Other.BufferDescriptors);
	Other.VAO = 0;
	return *this;
}

VertexArrayObject::~VertexArrayObject() {
	if (VAO == 0) return;
	for (auto& BufferDescriptor : BufferDescriptors) {
		GLCALL(glDeleteBuffers(1, &BufferDescriptor.VBO));
	}
	GLCALL(glDeleteVertexArrays(1, &VAO));
}

void VertexArrayObject::bind() {
	GLCALL(glBindVertexArray(VAO));
}

void VertexArrayObject::unbind() {
#ifndef NDEBUG
	GLCALL(glBindVertexArray(0));
#endif
}

void VertexArrayObject::DrawAs(GLenum mode) {
	bool HasInstanced = std::any_of(
		BufferDescriptors.begin(),
		BufferDescriptors.end(),
		[](const VertexBufferObjectDescriptor& d) {
			return d.Instancingdivisor != 0;
		}
	);

	GLsizei count = std::numeric_limits<GLsizei>::max();
	for (const auto& descriptor : BufferDescriptors) {
		if (descriptor.Instancingdivisor == 0 && descriptor.NumVerts < count) {
			count = descriptor.NumVerts;
		}
	}

	if (!HasInstanced) {
		GLCALL(glDrawArrays(mode,GLint(0), count));
		return;
	}
	GLsizei instancecount = 1;
	for (const auto& descriptor : BufferDescriptors) {
		if (descriptor.Instancingdivisor != 0) {
			instancecount = std::lcm(instancecount, descriptor.NumVerts);
		}
	}

	GLCALL(glDrawArraysInstanced(mode, GLint(0), count, instancecount));
}

void VertexBufferObjectDescriptor::PrepareVBO(GLuint& Position) {
	PrepareVBOVertexFunktion(Position, Instancingdivisor);
}

void TransformationVertex::PrepareVBO(GLuint& Position, GLuint Instancingdivisor) {
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(TransformationVertex), (void*)offsetof(TransformationVertex, Posx)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(TransformationVertex), (void*)offsetof(TransformationVertex, Sizex)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 1, GL_FLOAT, GL_FALSE, sizeof(TransformationVertex), (void*)offsetof(TransformationVertex, Rotation)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 1, GL_FLOAT, GL_FALSE, sizeof(TransformationVertex), (void*)offsetof(TransformationVertex, Scale)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
}

void CoordVertex::PrepareVBO(GLuint& Position, GLuint Instancingdivisor) {
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(CoordVertex), (void*)offsetof(CoordVertex, x)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
}

void TextureAndCoordVertex::PrepareVBO(GLuint& Position, GLuint Instancingdivisor) {
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(TextureAndCoordVertex), (void*)offsetof(TextureAndCoordVertex, x)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(TextureAndCoordVertex), (void*)offsetof(TextureAndCoordVertex, u)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
}


void CoordXYAndColourRGBVertex::PrepareVBO(GLuint& Position, GLuint Instancingdivisor) {
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(CoordXYAndColourRGBVertex), (void*)offsetof(CoordXYAndColourRGBVertex, x)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(CoordXYAndColourRGBVertex), (void*)offsetof(CoordXYAndColourRGBVertex, r)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
}


void CoordXYAndColourRGBAVertex::PrepareVBO(GLuint& Position, GLuint Instancingdivisor) {
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 2, GL_FLOAT, GL_FALSE, sizeof(CoordXYAndColourRGBAVertex), (void*)offsetof(CoordXYAndColourRGBAVertex, x)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 4, GL_FLOAT, GL_FALSE, sizeof(CoordXYAndColourRGBAVertex), (void*)offsetof(CoordXYAndColourRGBAVertex, r)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
}

void CoordXYZAndColourRGBVertex::PrepareVBO(GLuint& Position, GLuint Instancingdivisor) {
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(CoordXYZAndColourRGBVertex), (void*)offsetof(CoordXYZAndColourRGBVertex, x)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(CoordXYZAndColourRGBVertex), (void*)offsetof(CoordXYZAndColourRGBVertex, r)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
}


void CoordXYZAndColourRGBAVertex::PrepareVBO(GLuint& Position, GLuint Instancingdivisor) {
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(CoordXYZAndColourRGBAVertex), (void*)offsetof(CoordXYZAndColourRGBAVertex, x)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
	GLCALL(glEnableVertexAttribArray(Position));
	GLCALL(glVertexAttribPointer(Position, 4, GL_FLOAT, GL_FALSE, sizeof(CoordXYZAndColourRGBAVertex), (void*)offsetof(CoordXYZAndColourRGBAVertex, r)));
	GLCALL(glVertexAttribDivisor(Position++, Instancingdivisor));
}
