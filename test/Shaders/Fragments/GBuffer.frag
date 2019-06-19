#version 410

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;

    bool hasDiffuseTexture;
    sampler2D diffuseTexture;
};

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAmbientAlbedo;
layout (location = 3) out vec3 gDiffuseAlbedo;
layout (location = 4) out vec4 gSpecular;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

uniform Material material;

void main()
{
    vec4 texColor = vec4(1, 1, 1, 1);
    texColor = texture(material.diffuseTexture, fTexCoord);
    if (texColor.a < 0.1) discard;

    // store the fragment position vector in the first gbuffer texture
    gPosition = fPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(fNormal);
    // and the diffuse per-fragment color
    gAmbientAlbedo = (vec4(material.ambient, 1.0) * texColor).rgb;
    gDiffuseAlbedo = (vec4(material.diffuse, 1.0) * texColor).rgb;

    gSpecular = vec4(material.specular, material.shiness);
}