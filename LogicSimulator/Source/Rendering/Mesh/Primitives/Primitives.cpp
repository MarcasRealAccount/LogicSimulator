#include "Rendering/Mesh/Primitives/Primitives.h"

namespace MeshPrimitives {
	Mesh2D* rect2D;
	Mesh2D* line2D;
}

const Mesh2D& GetRect2DPrimitive() {
	if (MeshPrimitives::rect2D)
		return *MeshPrimitives::rect2D;

	MeshPrimitives::rect2D = new Mesh2D(false);
	MeshPrimitives::rect2D->vertices.push_back({ { 0.0f, 0.0f },  { 0.0f, 0.0f } });
	MeshPrimitives::rect2D->vertices.push_back({ { 1.0f, 0.0f },  { 1.0f, 0.0f } });
	MeshPrimitives::rect2D->vertices.push_back({ { 1.0f, 1.0f },  { 1.0f, 1.0f } });
	MeshPrimitives::rect2D->vertices.push_back({ { 0.0f, 1.0f },  { 0.0f, 1.0f } });

	MeshPrimitives::rect2D->indices.push_back(0);
	MeshPrimitives::rect2D->indices.push_back(1);
	MeshPrimitives::rect2D->indices.push_back(3);
	MeshPrimitives::rect2D->indices.push_back(1);
	MeshPrimitives::rect2D->indices.push_back(2);
	MeshPrimitives::rect2D->indices.push_back(3);

	return *MeshPrimitives::rect2D;
}

const Mesh2D& GetLine2DPrimitive() {
	if (MeshPrimitives::line2D)
		return *MeshPrimitives::line2D;

	MeshPrimitives::line2D = new Mesh2D(false);
	MeshPrimitives::line2D->renderMode = RenderMode::LINES;
	MeshPrimitives::line2D->vertices.push_back({ { 0.0f, 0.0f },  { 0.0f, 0.0f } });
	MeshPrimitives::line2D->vertices.push_back({ { 1.0f, 0.0f },  { 1.0f, 0.0f } });

	MeshPrimitives::line2D->indices.push_back(0);
	MeshPrimitives::line2D->indices.push_back(1);

	return *MeshPrimitives::line2D;
}