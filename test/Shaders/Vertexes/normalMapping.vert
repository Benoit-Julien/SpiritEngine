#version 410

layout (location = 0) in vec3 vertexPosition;// set the attr. number
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitTangent;

//out vec3 fNormal;
out vec3 fPosition;
out vec2 texCoord;
out mat3 tangentMatrix;

uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    vec3 norm = normalize(normalMatrix * vertexNormal);
    vec3 tang = normalize(normalMatrix * vertexTangent);
    vec3 bittang = normalize(normalMatrix * vertexBitTangent);

    tangentMatrix = mat3(tang.x, norm.x, bittang.x,
    tang.y, norm.y, bittang.y,
    tang.z, norm.z, bittang.z);

    texCoord = vertexTexCoord;
    //fNormal = normalize(normalMatrix * vertexNormal);
    vec4 pos = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    fPosition = pos.xyz;
    gl_Position = projectionMatrix * pos;
}
