#version 410

layout (location = 0) in vec3 vertexPosition;// set the attr. number
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

out vec3 fNormal;
out vec3 fPosition;
out vec2 texCoord;

uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    texCoord = vertexTexCoord;
    fNormal = normalize(normalMatrix * vertexNormal);
    vec4 pos = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    fPosition = pos.xyz;
    gl_Position = projectionMatrix * pos;
}
