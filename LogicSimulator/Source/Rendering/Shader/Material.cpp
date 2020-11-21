#include "Rendering/Shader/Material.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/Renderer.h"

#include <glm/glm.hpp>

#include "Rendering/Apis/OpenGL/Shader/OpenGLMaterialData.h"

Material::~Material() {
	if (this->materialData) {
		this->materialData->CleanUp();
		delete this->materialData;
	}
}

void Material::SetShader(Shader* shader) {
	this->shader = shader;
	this->uniforms.clear();

	if (this->shader) {
		for (auto uniform : this->shader->uniforms) {
			switch (uniform.second.first) {
			case UniformType::FLOAT:
				this->uniforms.insert({ uniform.first, Uniform<float>(0.0f) });
				break;
			case UniformType::FLOAT_VEC2:
				this->uniforms.insert({ uniform.first, Uniform<glm::fvec2>({ 0.0f, 0.0f }) });
				break;
			case UniformType::FLOAT_VEC3:
				this->uniforms.insert({ uniform.first, Uniform<glm::fvec3>({ 0.0f, 0.0f, 0.0f }) });
				break;
			case UniformType::FLOAT_VEC4:
				this->uniforms.insert({ uniform.first, Uniform<glm::fvec4>({ 0.0f, 0.0f, 0.0f, 0.0f }) });
				break;
			case UniformType::INT:
				this->uniforms.insert({ uniform.first, Uniform<int32_t>(0) });
				break;
			case UniformType::INT_VEC2:
				this->uniforms.insert({ uniform.first, Uniform<glm::ivec2>({ 0, 0 }) });
				break;
			case UniformType::INT_VEC3:
				this->uniforms.insert({ uniform.first, Uniform<glm::ivec3>({ 0, 0, 0 }) });
				break;
			case UniformType::INT_VEC4:
				this->uniforms.insert({ uniform.first, Uniform<glm::ivec4>({ 0, 0, 0, 0 }) });
				break;
			case UniformType::UINT:
				this->uniforms.insert({ uniform.first, Uniform<uint32_t>(0) });
				break;
			case UniformType::UINT_VEC2:
				this->uniforms.insert({ uniform.first, Uniform<glm::ivec2>({ 0, 0 }) });
				break;
			case UniformType::UINT_VEC3:
				this->uniforms.insert({ uniform.first, Uniform<glm::ivec3>({ 0, 0, 0 }) });
				break;
			case UniformType::UINT_VEC4:
				this->uniforms.insert({ uniform.first, Uniform<glm::ivec4>({ 0, 0, 0, 0 }) });
				break;
			case UniformType::FLOAT_MAT2:
				this->uniforms.insert({ uniform.first, Uniform<glm::fmat2>(glm::fmat2(1.0f)) });
				break;
			case UniformType::FLOAT_MAT3:
				this->uniforms.insert({ uniform.first, Uniform<glm::fmat3>(glm::fmat3(1.0f)) });
				break;
			case UniformType::FLOAT_MAT4:
				this->uniforms.insert({ uniform.first, Uniform<glm::fmat4>(glm::fmat4(1.0f)) });
				break;
			}
		}
	}
}

Shader* Material::GetShader() const {
	return this->shader;
}

MaterialData* Material::GetMaterialData(Renderer* renderer) {
	if (!this->materialData || !renderer->IsMaterialDataUsable(this->materialData)) {
		if (this->materialData) delete this->materialData;
		this->materialData = CreateCustomMaterialData(renderer->GetRendererType());
	}
	return this->materialData;
}

MaterialData* Material::CreateCustomMaterialData(RendererType rendererType) {
	switch (rendererType) {
	case RendererType::OPENGL:
		return new OpenGLMaterialData(this);
	default:
		return nullptr;
	}
}

MaterialData::MaterialData(Material* material)
	: material(material) {}

MaterialData::~MaterialData() {
	if (this->material) this->material->materialData = nullptr;
}

const std::unordered_map<std::string, std::any>& MaterialData::GetUniforms() const {
	return this->material->uniforms;
}