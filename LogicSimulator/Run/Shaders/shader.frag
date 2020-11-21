#version 150

in vec2 passUV;

out vec4 outColor;

uniform float time;

void main(void) {
	outColor = vec4(passUV, sin(time) * 0.5 + 0.5, 1.0);
}