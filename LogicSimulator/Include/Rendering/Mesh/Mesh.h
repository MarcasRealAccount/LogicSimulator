#pragma once

#include "Rendering/RendererType.h"

#include <stdint.h>
#include <vector>

enum class VertexAttribIndex : uint32_t {
	POSITION = 0,
	NORMAL = 1,
	UV = 2,
	SSBO_INDEX = 3,
	JOINT_INDICES = 3,
	JOINT_WEIGHTS = 4
};

enum class RenderMode : uint32_t {
	POINTS,
	LINE_STRIP,
	LINE_LOOP,
	LINES,
	LINE_STRIP_ADJANCENCY,
	LINES_ADJACENCY,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	TRIANGLES,
	TRIANGLE_STRIP_ADJACENCY,
	TRIANGLES_ADJACENCY,
	PATCHES
};

class Renderer;
struct MeshData;

struct Mesh {
public:
	Mesh(bool editable = true, bool dynamic = false);
	virtual ~Mesh();

	void MarkDirty();
	void ClearDirty();
	bool IsDirty();

	bool IsEditable();
	bool IsDynamic();

	MeshData* GetMeshData(Renderer* renderer);

	friend MeshData;

private:
	virtual MeshData* CreateCustomMeshData(RendererType type) = 0;

public:
	std::vector<uint32_t> indices;

	RenderMode renderMode = RenderMode::TRIANGLES;
	float lineWidth = 1.0f;

protected:
	bool dirty = true;
	bool editable;
	bool dynamic;

private:
	MeshData* meshData = nullptr;
};

struct MeshData {
public:
	MeshData(Mesh* mesh);
	virtual ~MeshData();

	virtual RendererType GetRendererType() const = 0;

	virtual void CleanUp() = 0;

protected:
	template <typename T>
	T* GetMesh() const { return reinterpret_cast<T*>(this->mesh); }

private:
	Mesh* mesh;
};