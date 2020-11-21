#include "Rendering/Apis/OpenGL/Mesh/OpenGLMesh.h"

OpenGLMeshData::OpenGLMeshData(Mesh* mesh)
	: MeshData(mesh) {}

GLenum OpenGLMeshData::GetRenderMode() const {
	switch (GetMesh<Mesh>()->renderMode) {
	case RenderMode::POINTS:
		return GL_POINTS;
	case RenderMode::LINE_STRIP:
		return GL_LINE_STRIP;
	case RenderMode::LINE_LOOP:
		return GL_LINE_LOOP;
	case RenderMode::LINES:
		return GL_LINES;
	case RenderMode::LINE_STRIP_ADJANCENCY:
		return GL_LINE_STRIP_ADJACENCY;
	case RenderMode::LINES_ADJACENCY:
		return GL_LINES_ADJACENCY;
	case RenderMode::TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	case RenderMode::TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;
	case RenderMode::TRIANGLES:
		return GL_TRIANGLES;
	case RenderMode::TRIANGLE_STRIP_ADJACENCY:
		return GL_TRIANGLE_STRIP_ADJACENCY;
	case RenderMode::TRIANGLES_ADJACENCY:
		return GL_TRIANGLES_ADJACENCY;
	case RenderMode::PATCHES:
		return GL_PATCHES;
	default:
		return GL_POINTS;
	}
}

RendererType OpenGLMeshData::GetRendererType() const {
	return RendererType::OPENGL;
}

bool OpenGLMeshData::HasIndices() {
	return this->hasIndices;
}

uint32_t OpenGLMeshData::GetVAO() {
	if (GetMesh<Mesh>()->IsDirty()) InitGLData();
	return this->vao;
}

void OpenGLMeshData::InitGLData() {
	if (GetCustomDataSize() == 0) {
		CleanUp();
		return;
	}

	Mesh* mesh = GetMesh<Mesh>();

	if (!this->vao) glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	bool reAssignedBuffers = false;
	this->currentVBO = 0;
	uint32_t newVBOCount = static_cast<uint32_t>(mesh->indices.size() > 0) + GetCustomVBOCount();
	if (this->numVBOs != newVBOCount) {
		if (this->vbos) {
			glDeleteBuffers(this->numVBOs, this->vbos);
			delete[] this->vbos;
		}
		this->numVBOs = newVBOCount;
		this->vbos = new uint32_t[this->numVBOs];
		glGenBuffers(this->numVBOs, this->vbos);
		reAssignedBuffers = true;
	}

	InitCustomGLData();

	if (mesh->indices.size() > 0) {
		BindNextVBO(GL_ELEMENT_ARRAY_BUFFER);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<uint32_t>(mesh->indices.size()) * sizeof(uint32_t), mesh->indices.data(), GL_DYNAMIC_DRAW);
		this->bufferSize = static_cast<uint32_t>(mesh->indices.size());
		this->hasIndices = true;
	} else {
		this->bufferSize = GetCustomDataSize();
		this->hasIndices = false;
	}

	glBindVertexArray(0);
	if (!mesh->IsEditable() || !mesh->IsDynamic()) {
		ClearCustomData();
		mesh->indices.clear();
	}
	mesh->ClearDirty();
}

void OpenGLMeshData::CleanUp() {
	if (this->vao) {
		glDeleteVertexArrays(1, &this->vao);
		this->vao = 0;
	}
	if (this->vbos) {
		glDeleteBuffers(this->numVBOs, this->vbos);
		delete[] this->vbos;
		this->vbos = nullptr;
	}
	this->currentVBO = 0;
	this->bufferSize = 0;
	this->hasIndices = false;
}


void OpenGLMeshData::BindNextVBO(GLenum bufferType) {
	glBindBuffer(bufferType, this->vbos[this->currentVBO++]);
}

void OpenGLMeshData::UnbindVBO(GLenum bufferType) {
	glBindBuffer(bufferType, 0);
}

void OpenGLMeshData::SetVertexAttribPointer(uint32_t index, uint32_t size, GLenum type, bool normalized, uint64_t stride, uint64_t offset) {
	glVertexAttribPointer(index, size, type, normalized, static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(index);
}

void OpenGLMeshData::SetVertexAttribIPointer(uint32_t index, uint32_t size, GLenum type, uint64_t stride, uint64_t offset) {
	glVertexAttribIPointer(index, size, type, static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(index);
}

void OpenGLMeshData::SetVertexAttribLPointer(uint32_t index, uint32_t size, GLenum type, uint64_t stride, uint64_t offset) {
	glVertexAttribLPointer(index, size, type, static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(index);
}