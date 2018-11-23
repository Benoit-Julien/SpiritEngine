#version 410
#define MAX_LIGHTS 32

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float intensity;

	vec3 position;
};

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float phong;
};

in vec3 fNormal;
in vec3 fPosition;

uniform uint lightNumber;
uniform Light lights[MAX_LIGHTS];
uniform Material material;

// We need to create at least one "out vec4" for setting color fragment colors
out vec4 fragColor; // r,g,b,a

vec3 CalcColor(Light light) {
  vec3 lightDir = light.position - fPosition;
  float distance = length(lightDir);
  distance = distance * distance;

  vec3 L = normalize(lightDir);
  vec3 N = fNormal;
  vec3 V = normalize(-vec3(fPosition));
  vec3 R = normalize(reflect(-L, N));

  vec3 ambient = material.ambient * light.ambient;
  vec3 diffuse = material.diffuse * light.diffuse * max(dot(N, L), 0.0) * light.intensity / distance;
  vec3 specular = material.specular * light.specular * pow(max(dot(V, R), 0.0), material.phong) * light.intensity / distance;

  return (ambient + diffuse + specular);
}

void main() {
	vec3 result = material.ambient;
	for (int i = 0; i < lightNumber; i++)
  	result += CalcColor(lights[i]);
  fragColor = vec4(result, 1.0);
}
