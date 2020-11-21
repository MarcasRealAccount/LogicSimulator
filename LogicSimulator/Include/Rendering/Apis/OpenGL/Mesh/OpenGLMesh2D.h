#pragma once

#include "Rendering/Apis/OpenGL/Mesh/OpenGLMesh.h"

struct Mesh2D;

struct OpenGLMesh2DData : public OpenGLMeshData {
public:
	OpenGLMesh2DData(Mesh2D* mesh);

	virtual uint32_t GetCustomVBOCount() override;
	virtual uint32_t GetCustomDataSize() override;
	virtual void InitCustomGLData() override;
	virtual void ClearCustomData() override;
};