#version 150

in vec2 inPosition;
in vec2 inUV;

out vec2 passUV;

uniform mat4 transformationMatrix;
uniform mat4 projectionViewMatrix;

void main(void) {
	gl_Position = projectionViewMatrix * transformationMatrix * vec4(inPosition, 0.0, 1.0);
	passUV = inUV;
}