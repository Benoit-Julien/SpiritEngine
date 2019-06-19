#version 410

struct Material {
    samplerCube textures[1];
};

uniform Material material;

in vec3 ReflectDir;

out vec4 fragColor;

void main() {
    vec4 cubeMapColor = texture(material.textures[0], ReflectDir);

    fragColor = cubeMapColor;
}
