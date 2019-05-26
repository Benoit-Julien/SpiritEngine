#version 410

struct Material {

    samplerCube textures[1];
};

uniform Material material;

in vec3 ReflectDir;
in vec3 RefractDirR;
in vec3 RefractDirG;
in vec3 RefractDirB;

in float ReflectFactor;

out vec4 fragColor;

void main() {
    vec3 reflectColor = vec3(texture(material.textures[0], ReflectDir));

    vec3 refractColor;
    refractColor.r = texture(material.textures[0], RefractDirR).r;
    refractColor.g = texture(material.textures[0], RefractDirG).g;
    refractColor.b = texture(material.textures[0], RefractDirB).b;

    fragColor = vec4(mix(refractColor, reflectColor, ReflectFactor), 1.0);
}
