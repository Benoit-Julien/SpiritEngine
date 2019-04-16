#version 410

struct Material {
    vec3 color;
    float reflectFactor;

    samplerCube textures[1];
};

uniform Material material;

in vec3 ReflectDir;

out vec4 fragColor;

void main() {
    vec4 cubeMapColor = texture(material.textures[0], ReflectDir);

    fragColor = vec4(mix(material.color, vec3(cubeMapColor), material.reflectFactor), 1.0);
}
