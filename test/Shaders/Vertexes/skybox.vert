#version 410

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 ReflectDir;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

void main() {
    ReflectDir = vertexPosition;
    mat4 viewM = mat4(mat3(viewMatrix));
    gl_Position = projectionMatrix * viewM * vec4(vertexPosition, 1.0);
}
