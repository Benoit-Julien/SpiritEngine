#version 410

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 ReflectDir;
out vec3 RefractDirR;
out vec3 RefractDirG;
out vec3 RefractDirB;

out float ReflectFactor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

uniform float EtaR;
uniform float EtaG;
uniform float EtaB;

const float n1 = 1.0003;
const float n2 = 1.52;

void main() {
    vec3 p = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    vec3 n = normalize(vec3(modelMatrix * vec4(vertexNormal, 1.0)));
    vec3 v = normalize(cameraPosition - p);

    ReflectDir = reflect(-v, n);
    RefractDirR = refract(-v, n, EtaR);
    RefractDirG = refract(-v, n, EtaG);
    RefractDirB = refract(-v, n, EtaB);

    // Schilick's approximation
    float r0 = pow(n1 - n2, 2) / pow(n1 + n2, 2);
    ReflectFactor = r0 + (1 - r0) * pow(1 - dot(v, n), 5);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
