#include "Rendering/Shader/Shader.h"
#include "Rendering/Renderer.h"
#include "Utilities/Config/ConfigManager.h"
#include "Utilities/Config/ConfigFile.h"

#include "Rendering/Apis/OpenGL/Shader/OpenGLShaderData.h"

extern EnumVector<UniformType> UniformTypeNames;

std::unordered_map<std::string, Shader*> Shader::loadedShaders;

Shader::Shader(const std::string& id)
	: id(id) {}

Shader::~Shader() {
	if (this->shaderData) {
		this->shaderData->CleanUp();
		delete this->shaderData;
	}
}

const std::string& Shader::GetId() const {
	return this->id;
}

void Shader::MarkDirty() {
	this->dirty = true;
}

void Shader::ClearDirty() {
	this->dirty = false;
}

bool Shader::IsDirty() const {
	return this->dirty;
}

uint32_t Shader::GetUniformLocation(const std::string& id) const {
	auto itr = this->uniforms.find(id);
	if (itr != this->uniforms.end()) return itr->second.second;
	return 0;
}

ShaderData* Shader::GetShaderData(Renderer* renderer) {
	if (!this->shaderData || !renderer->IsShaderDataUsable(this->shaderData)) {
		if (this->shaderData) delete this->shaderData;
		this->shaderData = CreateCustomShaderData(renderer->GetRendererType());
	}
	return this->shaderData;
}

void Shader::LoadAttributesAndUniforms() {
	this->attributes.clear();
	this->uniforms.clear();
	ConfigFile* shaderConfig = ConfigManager::GetConfigFilePath("Shaders/" + this->id);
	ConfigSection* shaderAttributes = shaderConfig->GetSection("Attributes");
	if (shaderAttributes) {
		for (auto& attribute : shaderAttributes->GetConfigs()) {
			uint32_t index = shaderAttributes->GetConfigTyped<uint32_t>(attribute.first, 0);
			this->attributes.insert({ attribute.first, index });
		}
	}
	ConfigSection* shaderUniforms = shaderConfig->GetSection("Uniforms");
	if (shaderUniforms) {
		for (auto& uniform : shaderUniforms->GetConfigs()) {
			this->uniforms.insert({ uniform.first, { shaderUniforms->GetConfigEnum(uniform.first, UniformType::FLOAT, UniformTypeNames), 0} });
		}
	}
	delete shaderConfig;
}

ShaderData* Shader::CreateCustomShaderData(RendererType rendererType) {
	switch (rendererType) {
	case RendererType::OPENGL:
		return new OpenGLShaderData(this);
	default:
		return nullptr;
	}
}

Shader* Shader::GetShader(const std::string& id) {
	auto itr = Shader::loadedShaders.find(id);
	if (itr != Shader::loadedShaders.end())
		return itr->second;

	Shader* shader = new Shader(id);
	shader->LoadAttributesAndUniforms();
	Shader::loadedShaders.insert({ id, shader });
	return shader;
}

void Shader::CleanUpShaders() {
	for (auto& shader : Shader::loadedShaders) {
		if (shader.second->shaderData) {
			delete shader.second;
		}
	}

	Shader::loadedShaders.clear();
}

ShaderData::ShaderData(Shader* shader)
	: shader(shader) {}

ShaderData::~ShaderData() {
	if (this->shader) this->shader->shaderData = nullptr;
}

const std::unordered_map<std::string, uint32_t>& ShaderData::GetAttributes() const {
	return this->shader->attributes;
}

void ShaderData::SetAttributeIndex(const std::string& id, uint32_t index) {
	auto itr = this->shader->attributes.find(id);
	if (itr != this->shader->attributes.end()) itr->second = index;
	else this->shader->attributes.insert({ id, index });
}

void ShaderData::SetUniformTypeAndLocation(const std::string& id, UniformType type, uint32_t location) {
	auto itr = this->shader->uniforms.find(id);
	if (itr != this->shader->uniforms.end()) {
		itr->second.first = type;
		itr->second.second = location;
	} else {
		this->shader->uniforms.insert({ id, { type, location } });
	}
}

EnumVector<UniformType> UniformTypeNames = {
	{ UniformType::FLOAT, "Float" },
	{ UniformType::FLOAT_VEC2, "FVec2" },
	{ UniformType::FLOAT_VEC3, "FVec3" },
	{ UniformType::FLOAT_VEC4, "FVec4" },
	{ UniformType::INT, "Int" },
	{ UniformType::INT_VEC2, "IVec2" },
	{ UniformType::INT_VEC3, "IVec3" },
	{ UniformType::INT_VEC4, "IVec4" },
	{ UniformType::UINT, "UInt" },
	{ UniformType::UINT_VEC2, "UVec2" },
	{ UniformType::UINT_VEC3, "UVec3" },
	{ UniformType::UINT_VEC4, "UVec4" },
	{ UniformType::FLOAT_MAT2, "FMat2" },
	{ UniformType::FLOAT_MAT3, "FMat3" },
	{ UniformType::FLOAT_MAT4, "FMat4" }
};