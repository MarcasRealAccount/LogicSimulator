#pragma once

#include "Rendering/RendererType.h"

#include <string>
#include <unordered_map>
#include <any>

enum class TriangleFace : uint32_t {
	BACK,
	FRONT,
	FRONT_AND_BACK
};

enum class BlendFunc : uint32_t {
	ZERO,
	ONE,
	SRC_COLOR,
	ONE_MINUS_SRC_COLOR,
	DST_COLOR,
	ONE_MINUS_DST_COLOR,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
	DST_ALPHA,
	ONE_MINUS_DST_ALPHA,
	CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR,
	CONSTANT_ALPHA,
	ONE_MINUS_CONSTANT_ALPHA,
	SRC_ALPHA_SATURATE,
	SRC1_COLOR,
	ONE_MINUS_SRC1_COLOR,
	SRC1_ALPHA,
	ONE_MINUS_SRC1_ALPHA
};

enum class PolygonMode : uint32_t {
	POINT,
	LINE,
	FILL
};

class Shader;
class MaterialData;
class Renderer;

template <typename T>
struct Uniform;

class Material {
public:
	~Material();

	void SetShader(Shader* shader);
	Shader* GetShader() const;

	template <typename T>
	Uniform<T>* GetUniform(const std::string& id) {
		if (!this->shader) return nullptr;

		auto itr = this->uniforms.find(id);
		if (itr != this->uniforms.end()) return std::any_cast<Uniform<T>>(&itr->second);
		return nullptr;
	}

	MaterialData* GetMaterialData(Renderer* renderer);

	friend MaterialData;

private:
	virtual MaterialData* CreateCustomMaterialData(RendererType rendererType);

public:
	struct {
		bool enabled = true;
		TriangleFace face = TriangleFace::BACK;
	} cullMode;

	bool depthTest = true;

	struct {
		bool enabled = true;
		BlendFunc srcFunc = BlendFunc::SRC_ALPHA;
		BlendFunc dstFunc = BlendFunc::DST_ALPHA;
	} blendFunc;

	struct {
		bool enabled = true;
		TriangleFace face = TriangleFace::FRONT_AND_BACK;
		PolygonMode mode = PolygonMode::FILL;
	} polygonMode;

protected:
	std::unordered_map<std::string, std::any> uniforms;

private:
	Shader* shader = nullptr;
	MaterialData* materialData = nullptr;
};

class MaterialData {
public:
	MaterialData(Material* material);
	virtual ~MaterialData();

	virtual void CleanUp() = 0;

	virtual RendererType GetRendererType() const = 0;

	friend Renderer;

protected:
	template <typename T>
	T* GetMaterial() const { return reinterpret_cast<T*>(this->material); }

	const std::unordered_map<std::string, std::any>& GetUniforms() const;

private:
	Material* material;
};