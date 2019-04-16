#version 410

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 ReflectDir;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

void main() {
    vec3 p = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    vec3 n = normalize(vec3(modelMatrix * vec4(vertexNormal, 1.0)));
    vec3 v = normalize(cameraPosition - p);

    ReflectDir = reflect(-v, n);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
