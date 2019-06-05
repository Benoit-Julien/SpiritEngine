R"=====(
#version 410

layout (location = 0) in vec3 vertexPosition;// set the attr. number
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main() {
    fTexCoord = vertexTexCoord;
    fNormal = normalize(normalMatrix * vertexNormal);
    vec4 pos = modelMatrix * vec4(vertexPosition, 1.0);
    fPosition = pos.xyz;
    gl_Position = projectionMatrix * viewMatrix * pos;
}
)=====";