#version 410

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;

    sampler2D texture_diffuse;
};

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec4 gSpecular;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

uniform Material material;

void main()
{
    vec4 texColor = texture(material.texture_diffuse, fTexCoord);
    if (texColor.a < 0.1) discard;

    // store the fragment position vector in the first gbuffer texture
    gPosition = fPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(fNormal);
    // and the diffuse per-fragment color
    gAlbedo = (vec4(material.ambient * material.diffuse, 1.0) * texColor).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gSpecular.rgb = material.specular;
    gSpecular.a = material.shiness;
}