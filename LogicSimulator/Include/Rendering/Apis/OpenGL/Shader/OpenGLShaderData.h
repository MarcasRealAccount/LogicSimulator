#pragma once

#include "Rendering/Shader/Shader.h"
#include "Rendering/Apis/OpenGL/GLHelpers.h"

class OpenGLRenderer;

class OpenGLShaderData : public ShaderData {
public:
	OpenGLShaderData(Shader* shader);

	virtual RendererType GetRendererType() const override;
	virtual void CleanUp() override;

	uint32_t GetProgramID();
	void InitGLData();

	void Start();
	void Stop();

	friend OpenGLRenderer;

protected:
	uint32_t LoadShader(ShaderType type);

protected:
	static uint32_t GetShaderTypeId(ShaderType type);
	static const char* GetShaderTypeName(ShaderType type);
	static const char* GetShaderTypeExtensionName(ShaderType type);

	static UniformType GetUniformType(GLenum type);

private:
	uint32_t programID = 0;

	std::unordered_map<ShaderType, bool> shaders;
};