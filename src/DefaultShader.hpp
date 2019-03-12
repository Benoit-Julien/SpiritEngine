#ifndef COMPUTERGRAPHICS1_DEFAULTSHADER_HPP
#define COMPUTERGRAPHICS1_DEFAULTSHADER_HPP

#include <string>

static const std::string default_vertex_shader =
				"#version 410\n"
				"layout (location = 0) in vec3 vertexPosition; // set the attr. number\n"
				"layout (location = 1) in vec3 vertexNormal;\n"
				"out vec3 fNormal;\n"
				"out vec3 fPosition;\n"
				"uniform mat3 normalMatrix;\n"
				"uniform mat4 modelMatrix;\n"
				"uniform mat4 viewMatrix;\n"
				"uniform mat4 projectionMatrix;\n"
				"void main() {\n"
				"fNormal = normalize(normalMatrix * vertexNormal);\n"
				"vec4 pos = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);\n"
				"fPosition = pos.xyz;\n"
				"gl_Position = projectionMatrix * pos;\n"
				"}";

static const std::string default_fragment_shader =
				"#version 410\n"
				"#define MAX_LIGHTS 32\n"
				"struct Light {\n"
				"vec3 ambient;\n"
				"vec3 diffuse;\n"
				"vec3 specular;\n"
				"float intensity;\n"
				"vec3 position;\n"
				"};\n"
				"struct Material {\n"
				"vec3 ambient;\n"
				"vec3 diffuse;\n"
				"vec3 specular;\n"
				"float shiness;\n"
				"};\n"
				"in vec3 fNormal;\n"
				"in vec3 fPosition;\n"
				"uniform uint lightNumber;\n"
				"uniform Light lights[MAX_LIGHTS];\n"
				"uniform Material material;\n"
				"out vec4 fragColor; // r,g,b,a\n"
				"vec3 CalcColor(Light light) {\n"
				"vec3 lightDir = light.position - fPosition;\n"
				"float distance = length(lightDir);\n"
				"distance = distance * distance;\n"
				"vec3 L = normalize(lightDir);\n"
				"vec3 N = fNormal;\n"
				"vec3 V = normalize(-vec3(fPosition));\n"
				"vec3 H = normalize(V + L);\n"
				"vec3 ambient = material.ambient * light.ambient;\n"
				"vec3 diffuse = material.diffuse * light.diffuse * max(dot(N, L), 0.0) * light.intensity / distance;\n"
				"vec3 specular = material.specular * light.specular * pow(max(dot(H, N), 0.0), material.shiness) * light.intensity / distance;\n"
				"return (ambient + diffuse + specular);\n"
				"}\n"
				"void main() {\n"
				"vec3 result = material.ambient;\n"
				"for (int i = 0; i < lightNumber; i++)\n"
				"result += CalcColor(lights[i]);\n"
				"fragColor = vec4(result, 1);\n"
				"}";

#endif /* !COMPUTERGRAPHICS1_DEFAULTSHADER_HPP */
