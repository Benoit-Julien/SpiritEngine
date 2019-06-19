#version 410

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shiness;
};

uniform Material material;

out vec4 fragColor; // r,g,b,a

void main() {
	fragColor = vec4(material.ambient, 1);
}
