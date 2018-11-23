#version 410

in vec3 fNormal;
in vec3 fPosition;
in vec3 fcolor; // getting color date from Vertex shader

// We need to create at least one "out vec4" for setting color fragment colors
out vec4 fragColor; // r,g,b,a
void main() {
	fragColor = vec4(fcolor, 1.0);
}
