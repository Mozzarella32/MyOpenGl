#pragma once

#include "pch.hpp"

#include "Utilities.hpp"

struct CoordVertex {
	float x;
	float y;

	bool operator ==(const CoordVertex& Other) const = default;

	static void PrepareVBO(GLuint& Position, GLuint Instancingdivisor);
};

struct TextureAndCoordVertex {
	float x;
	float y;

	float u;
	float v;

	bool operator ==(const TextureAndCoordVertex& Other) const = default;

	static void PrepareVBO(GLuint& Position, GLuint Instancingdivisor);
};

struct TransformationVertex {
	float Posx;
	float Posy;
	float Sizex;
	float Sizey;
	float Rotation;
	float Scale;

	bool operator ==(const TransformationVertex& Other) const = default;

	static void PrepareVBO(GLuint& Position, GLuint Instancingdivisor);
};


struct CoordXYAndColourRGBVertex {
	float x;
	float y;

	float r;
	float g;
	float b;

	bool operator ==(const CoordXYAndColourRGBVertex& Other) const = default;

	static void PrepareVBO(GLuint& Position, GLuint Instancingdivisor);
};


struct CoordXYAndColourRGBAVertex {
	float x;
	float y;

	float r;
	float g;
	float b;
	float a;

	bool operator ==(const CoordXYAndColourRGBAVertex& Other) const = default;

	static void PrepareVBO(GLuint& Position, GLuint Instancingdivisor);
};


struct CoordXYZAndColourRGBVertex {
	float x;
	float y;
	float z;

	float r;
	float g;
	float b;

	bool operator ==(const CoordXYZAndColourRGBVertex& Other) const = default;

	static void PrepareVBO(GLuint& Position, GLuint Instancingdivisor);
};


struct CoordXYZAndColourRGBAVertex {
	float x;
	float y;
	float z;

	float r;
	float g;
	float b;
	float a;

	bool operator ==(const CoordXYZAndColourRGBAVertex& Other) const = default;

	static void PrepareVBO(GLuint& Position, GLuint Instancingdivisor);
};

class VertexBufferObjectDescriptor {
public:
	GLenum Usage;
	GLuint Instancingdivisor;

	GLsizei NumVerts = 0;

	std::function<void(GLuint&, GLuint)> PrepareVBOVertexFunktion;

	void PrepareVBO(GLuint& Position);

	GLuint VBO = GLuint(-1);
	template<class VertexType>
	//The VertexType is just a dummy object wich is unused, becaus Constructors can't have explicit template parameters
	VertexBufferObjectDescriptor(GLenum Usage, VertexType, GLuint Instancingdivisor = 0)
		:Usage(Usage), Instancingdivisor(Instancingdivisor), PrepareVBOVertexFunktion(VertexType::PrepareVBO)
	{}

	//VertexBufferObjectDescriptor(const VertexBufferObjectDescriptor&) = delete;
	//VertexBufferObjectDescriptor(VertexBufferObjectDescriptor&&) = delete;
	//VertexBufferObjectDescriptor& operator=(const VertexBufferObjectDescriptor&) = delete;
	//VertexBufferObjectDescriptor& operator=(VertexBufferObjectDescriptor&&) = delete;

};


class VertexArrayObject {
private:
	GLuint VAO = 0;
	std::vector<VertexBufferObjectDescriptor> BufferDescriptors;
public:

	VertexArrayObject(std::vector<VertexBufferObjectDescriptor> InitialBufferDescriptors);

	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	VertexArrayObject(VertexArrayObject&& Other) noexcept;

	VertexArrayObject& operator=(VertexArrayObject&& Other) noexcept;

	virtual ~VertexArrayObject();

	void bind();

	void unbind();

	void DrawAs(GLenum mode);

	template<class VertexType>
	void ReplaceVertexBuffer(const std::vector<VertexType>& Vert, size_t BufferIndex) {
		//if (Vert.empty()) return;

		assert(BufferIndex < BufferDescriptors.size());
		auto& Descriptor = BufferDescriptors[BufferIndex];

		if (Descriptor.NumVerts == Vert.size()) {
			GLCALL(glNamedBufferSubData(Descriptor.VBO, 0, Descriptor.NumVerts * sizeof(VertexType), Vert.data()));
		}
		else {
			Descriptor.NumVerts = Vert.size();
			GLCALL(glNamedBufferData(Descriptor.VBO, Descriptor.NumVerts * sizeof(VertexType), Vert.data(), Descriptor.Usage));
		}
	}
};


template<class VertexType>
struct BufferedVertexVec {
private:
	std::vector<VertexType> Vertices;
	bool Dirty = true;
public:

	void replaceBuffer(VertexArrayObject& VAO, size_t BufferIndex, bool ClearDirty = true) {
		if (!Dirty)return;
		VAO.ReplaceVertexBuffer(Vertices, BufferIndex);
		if (ClearDirty)Dirty = false;
	}

	void clear() {
		Dirty = true;
		Vertices.clear();
	}

	void append(const VertexType& Vertex) {
		Dirty = true;
		Vertices.push_back(Vertex);
	}

	template<typename ...Args>
	void emplace(Args&&... args) {
		Dirty = true;
		Vertices.emplace_back(std::forward<Args>(args)...);
	}

	void append(const std::vector<VertexType>& Vertex) {
		Dirty = true;
		Vertices.insert(Vertices.end(), Vertex.begin(), Vertex.end());
	}

	template<typename ...Args>
	void appendOther(Args&&... Other) {
		Dirty = true;
		Vertices.reserve(Vertices.size() + (Other.Vertices.size() + ...));
		(Append(Other.Vertices), ...);
	}

	bool empty() const {
		return Vertices.empty();
	}

	size_t size() const {
		return Vertices.size();
	}
};

//struct IndexBufferObject {
//	IndexBufferObject(void* data, uint32_t numIndices, uint8_t elementSize) {
//		glGenBuffers(1, &bufferId);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW);
//	}
//
//	virtual ~IndexBufferObject() {
//		glDeleteBuffers(1, &bufferId);
//	}
//
//	void bind() {
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
//	}
//
//	void unbind() {
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//
//private:
//	GLuint bufferId;
//};
