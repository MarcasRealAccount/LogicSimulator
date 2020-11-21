#pragma once

#include "Rendering/Mesh/Mesh.h"

#include "Rendering/Apis/OpenGL/GLHelpers.h"

class OpenGLRenderer;

struct OpenGLMeshData : public MeshData {
public:
	OpenGLMeshData(Mesh* mesh);

	GLenum GetRenderMode() const;
	virtual RendererType GetRendererType() const override;

	virtual uint32_t GetCustomVBOCount() = 0;
	virtual uint32_t GetCustomDataSize() = 0;
	virtual void InitCustomGLData() = 0;
	virtual void ClearCustomData() = 0;

	bool HasIndices();
	uint32_t GetVAO();
	void InitGLData();

	virtual void CleanUp() override;

	friend OpenGLRenderer;

protected:
	void BindNextVBO(GLenum bufferType);
	void UnbindVBO(GLenum bufferType);

	void SetVertexAttribPointer(uint32_t index, uint32_t size, GLenum type, bool normalized, uint64_t stride, uint64_t offset);
	void SetVertexAttribIPointer(uint32_t index, uint32_t size, GLenum type, uint64_t stride, uint64_t offset);
	void SetVertexAttribLPointer(uint32_t index, uint32_t size, GLenum type, uint64_t stride, uint64_t offset);

protected:
	uint32_t vao = 0;
	uint8_t numVBOs = 0;
	uint8_t currentVBO = 0;
	uint32_t* vbos = nullptr;
	uint32_t bufferSize = 0;
	bool hasIndices = false;
};