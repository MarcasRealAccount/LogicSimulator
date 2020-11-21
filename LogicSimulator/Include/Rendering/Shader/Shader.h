#pragma once

#include "Rendering/Shader/Uniform.h"
#include "Rendering/RendererType.h"

#include <stdint.h>
#include <string>
#include <unordered_map>

enum class ShaderType : uint32_t {
	COMPUTE,
	VERTEX,
	TESS_CONTROL,
	TESS_EVAL,
	GEOMETRY,
	FRAGMENT
};

class Renderer;
class Material;
class ShaderData;

class Shader {
public:
	Shader(const std::string& id);
	virtual ~Shader();

	const std::string& GetId() const;

	void MarkDirty();
	void ClearDirty();
	bool IsDirty() const;

	uint32_t GetUniformLocation(const std::string& id) const;

	ShaderData* GetShaderData(Renderer* renderer);

	friend ShaderData;
	friend Material;

private:
	void LoadAttributesAndUniforms();
	virtual ShaderData* CreateCustomShaderData(RendererType rendererType);

public:
	static Shader* GetShader(const std::string& id);
	static void CleanUpShaders();

protected:
	std::unordered_map<std::string, uint32_t> attributes;
	std::unordered_map<std::string, std::pair<UniformType, uint32_t>> uniforms;

private:
	std::string id;
	bool dirty = true;

	ShaderData* shaderData = nullptr;

private:
	static std::unordered_map<std::string, Shader*> loadedShaders;
};

class ShaderData {
public:
	ShaderData(Shader* shader);
	virtual ~ShaderData();

	virtual RendererType GetRendererType() const = 0;
	virtual void CleanUp() {};

protected:
	template <typename T>
	T* GetShader() const { return reinterpret_cast<T*>(this->shader); }

	const std::unordered_map<std::string, uint32_t>& GetAttributes() const;

	void SetAttributeIndex(const std::string& id, uint32_t index);
	void SetUniformTypeAndLocation(const std::string& id, UniformType type, uint32_t location);

private:
	Shader* shader;
};