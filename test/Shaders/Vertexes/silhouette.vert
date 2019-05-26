#version 410

layout (location = 0) in vec3 vertexPosition; // set the attr. number
layout (location = 1) in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float offset;

void main() {
	//vec3 fNormal = normalize(normalMatrix * vertexNormal);
	//pos : position in camera coord.
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition + vertexNormal * offset, 1.0);
}
