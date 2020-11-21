#include "Rendering/Apis/OpenGL/Shader/OpenGLMaterialData.h"
#include "Rendering/Shader/Shader.h"

#include <glm/glm.hpp>

OpenGLMaterialData::OpenGLMaterialData(Material* material)
	: MaterialData(material) {}

RendererType OpenGLMaterialData::GetRendererType() const {
	return RendererType::OPENGL;
}

void OpenGLMaterialData::CleanUp() {}

GLenum OpenGLMaterialData::GetCullFace() const {
	return GetGLCullFace(GetMaterial<Material>()->cullMode.face);
}

GLenum OpenGLMaterialData::GetSrcBlendFunc() const {
	return GetGLBlendFunc(GetMaterial<Material>()->blendFunc.srcFunc);
}

GLenum OpenGLMaterialData::GetDstBlendFunc() const {
	return GetGLBlendFunc(GetMaterial<Material>()->blendFunc.dstFunc);
}

GLenum OpenGLMaterialData::GetPolygonModeFace() const {
	return GetGLCullFace(GetMaterial<Material>()->polygonMode.face);
}

GLenum OpenGLMaterialData::GetPolygonMode() const {
	return GetGLPolygonMode(GetMaterial<Material>()->polygonMode.mode);
}

void OpenGLMaterialData::SetAllUniforms() {
	if (!GetMaterial<Material>()->GetShader()) return;

	for (auto& uniform : GetUniforms()) {
		const std::string& id = uniform.first;
		uint32_t location = GetMaterial<Material>()->GetShader()->GetUniformLocation(id);
		if (uniform.second.type() == typeid(Uniform<float>)) {
			const Uniform<float>* uniformFloat = std::any_cast<Uniform<float>>(&uniform.second);
			glUniform1f(location, uniformFloat->value);
		} else if (uniform.second.type() == typeid(Uniform<glm::fvec2>)) {
			const Uniform<glm::fvec2>* uniformVec2f = std::any_cast<Uniform<glm::fvec2>>(&uniform.second);
			glUniform2f(location, uniformVec2f->value.x, uniformVec2f->value.y);
		} else if (uniform.second.type() == typeid(Uniform<glm::fvec3>)) {
			const Uniform<glm::fvec3>* uniformVec3f = std::any_cast<Uniform<glm::fvec3>>(&uniform.second);
			glUniform3f(location, uniformVec3f->value.x, uniformVec3f->value.y, uniformVec3f->value.z);
		} else if (uniform.second.type() == typeid(Uniform<glm::fvec4>)) {
			const Uniform<glm::fvec4>* uniformVec4f = std::any_cast<Uniform<glm::fvec4>>(&uniform.second);
			glUniform4f(location, uniformVec4f->value.x, uniformVec4f->value.y, uniformVec4f->value.z, uniformVec4f->value.w);
		} else if (uniform.second.type() == typeid(Uniform<int32_t>)) {
			const Uniform<int32_t>* uniformInt = std::any_cast<Uniform<int32_t>>(&uniform.second);
			glUniform1i(location, uniformInt->value);
		} else if (uniform.second.type() == typeid(Uniform<glm::ivec2>)) {
			const Uniform<glm::ivec2>* uniformVec2i = std::any_cast<Uniform<glm::ivec2>>(&uniform.second);
			glUniform2i(location, uniformVec2i->value.x, uniformVec2i->value.y);
		} else if (uniform.second.type() == typeid(Uniform<glm::ivec3>)) {
			const Uniform<glm::ivec3>* uniformVec3i = std::any_cast<Uniform<glm::ivec3>>(&uniform.second);
			glUniform3i(location, uniformVec3i->value.x, uniformVec3i->value.y, uniformVec3i->value.z);
		} else if (uniform.second.type() == typeid(Uniform<glm::ivec4>)) {
			const Uniform<glm::ivec4>* uniformVec4i = std::any_cast<Uniform<glm::ivec4>>(&uniform.second);
			glUniform4i(location, uniformVec4i->value.x, uniformVec4i->value.y, uniformVec4i->value.z, uniformVec4i->value.w);
		} else if (uniform.second.type() == typeid(Uniform<uint32_t>)) {
			const Uniform<uint32_t>* uniformInt = std::any_cast<Uniform<uint32_t>>(&uniform.second);
			glUniform1ui(location, uniformInt->value);
		} else if (uniform.second.type() == typeid(Uniform<glm::uvec2>)) {
			const Uniform<glm::uvec2>* uniformVec2u = std::any_cast<Uniform<glm::uvec2>>(&uniform.second);
			glUniform2ui(location, uniformVec2u->value.x, uniformVec2u->value.y);
		} else if (uniform.second.type() == typeid(Uniform<glm::uvec3>)) {
			const Uniform<glm::uvec3>* uniformVec3u = std::any_cast<Uniform<glm::uvec3>>(&uniform.second);
			glUniform3ui(location, uniformVec3u->value.x, uniformVec3u->value.y, uniformVec3u->value.z);
		} else if (uniform.second.type() == typeid(Uniform<glm::uvec4>)) {
			const Uniform<glm::uvec4>* uniformVec4u = std::any_cast<Uniform<glm::uvec4>>(&uniform.second);
			glUniform4ui(location, uniformVec4u->value.x, uniformVec4u->value.y, uniformVec4u->value.z, uniformVec4u->value.w);
		} else if (uniform.second.type() == typeid(Uniform<glm::fmat2>)) {
			const Uniform<glm::fmat2>* uniformMat2f = std::any_cast<Uniform<glm::fmat2>>(&uniform.second);
			glUniformMatrix2fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&uniformMat2f->value));
		} else if (uniform.second.type() == typeid(Uniform<glm::fmat3>)) {
			const Uniform<glm::fmat3>* uniformMat3f = std::any_cast<Uniform<glm::fmat3>>(&uniform.second);
			glUniformMatrix3fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&uniformMat3f->value));
		} else if (uniform.second.type() == typeid(Uniform<glm::fmat4>)) {
			const Uniform<glm::fmat4>* uniformMat4f = std::any_cast<Uniform<glm::fmat4>>(&uniform.second);
			glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&uniformMat4f->value));
		}
	}
}

GLenum OpenGLMaterialData::GetGLCullFace(TriangleFace face) {
	switch (face) {
	case TriangleFace::BACK:
		return GL_BACK;
	case TriangleFace::FRONT:
		return GL_FRONT;
	case TriangleFace::FRONT_AND_BACK:
		return GL_FRONT_AND_BACK;
	default:
		return GL_BACK;
	}
}

GLenum OpenGLMaterialData::GetGLBlendFunc(BlendFunc blendFunc) {
	switch (blendFunc) {
	case BlendFunc::ZERO:
		return GL_ZERO;
	case BlendFunc::ONE:
		return GL_ONE;
	case BlendFunc::SRC_COLOR:
		return GL_SRC_COLOR;
	case BlendFunc::ONE_MINUS_SRC_COLOR:
		return GL_ONE_MINUS_SRC_COLOR;
	case BlendFunc::DST_COLOR:
		return GL_DST_COLOR;
	case BlendFunc::ONE_MINUS_DST_COLOR:
		return GL_ONE_MINUS_DST_COLOR;
	case BlendFunc::SRC_ALPHA:
		return GL_SRC_ALPHA;
	case BlendFunc::ONE_MINUS_SRC_ALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	case BlendFunc::DST_ALPHA:
		return GL_DST_ALPHA;
	case BlendFunc::ONE_MINUS_DST_ALPHA:
		return GL_ONE_MINUS_DST_ALPHA;
	case BlendFunc::CONSTANT_COLOR:
		return GL_CONSTANT_COLOR;
	case BlendFunc::ONE_MINUS_CONSTANT_COLOR:
		return GL_ONE_MINUS_CONSTANT_COLOR;
	case BlendFunc::CONSTANT_ALPHA:
		return GL_CONSTANT_ALPHA;
	case BlendFunc::ONE_MINUS_CONSTANT_ALPHA:
		return GL_ONE_MINUS_CONSTANT_ALPHA;
	case BlendFunc::SRC_ALPHA_SATURATE:
		return GL_SRC_ALPHA_SATURATE;
	case BlendFunc::SRC1_COLOR:
		return GL_SRC1_COLOR;
	case BlendFunc::ONE_MINUS_SRC1_COLOR:
		return GL_ONE_MINUS_SRC1_COLOR;
	case BlendFunc::SRC1_ALPHA:
		return GL_SRC1_ALPHA;
	case BlendFunc::ONE_MINUS_SRC1_ALPHA:
		return GL_ONE_MINUS_SRC1_ALPHA;
	default:
		return GL_ONE;
	}
}

GLenum OpenGLMaterialData::GetGLPolygonMode(PolygonMode polygonMode) {
	switch (polygonMode) {
	case PolygonMode::POINT:
		return GL_POINT;
	case PolygonMode::LINE:
		return GL_LINE;
	case PolygonMode::FILL:
		return GL_FILL;
	default:
		return GL_FILL;
	}
}