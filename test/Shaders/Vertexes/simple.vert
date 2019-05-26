#version 410

layout (location = 0) in vec3 vertexPosition;// set the attr. number
layout (location = 1) in vec3 vertexNormal;

out vec3 fNormal;
out vec3 fPosition;
out vec4 fragPosLightSpace;

// getting from app (const int shader, you are not allowes to change it)
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 lightMVP;

void main() {
    fNormal = normalize(normalMatrix * vertexNormal);
    //pos : position in camera coord.
    vec4 pos = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    fPosition = pos.xyz;
    fragPosLightSpace = lightMVP * vec4(vertexPosition, 1.0);
    gl_Position = projectionMatrix * pos;
}