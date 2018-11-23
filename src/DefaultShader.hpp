#ifndef COMPUTERGRAPHICS1_DEFAULTSHADER_HPP
#define COMPUTERGRAPHICS1_DEFAULTSHADER_HPP

#include <string>

static const std::string default_vertex_shader =
				"#version 410\n"
				"layout (location = 0) in vec3 vertexPosition; // set the attr. number\n"
				"layout (location = 1) in vec3 vertexNormal;\n"
				"out vec3 fNormal;\n"
				"out vec3 fPosition;\n"
				"out vec3 fcolor; // for sending vertex color to Fragment shader\n"
				"const mat3 normalMatrix = mat3(1.0);\n"
				"uniform mat4 modelMatrix; // getting from app (const int shader, you are not allowes to change it)\n"
				"uniform mat4 viewMatrix;\n"
				"uniform mat4 projectionMatrix;\n"
				"const vec4 lightLocation = vec4(0, 10, 0, 1);\n"
				"const vec3 Kd = vec3(1, 1, 0);\n"
				"const vec3 Id = vec3(1, 1, 1);\n"
				"const vec3 Ka = vec3(0.2, 0.2, 0);\n"
				"const vec3 Ia = vec3(1, 1, 1);\n"
				"const vec3 Ks = vec3(0.3, 0.3, 0);\n"
				"const vec3 Is = vec3(1, 1, 1);\n"
				"const float p = 10.0; // phong exponents\n"
				"void main() {\n"
				"  fNormal = normalize(normalMatrix * vertexNormal);\n"
				"  //pos : position in camera coord.\n"
				"  vec4 pos = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);\n"
				"  vec3 L = normalize(vec3(lightLocation - pos));\n"
				"  vec3 N = fNormal;\n"
				"  vec3 V = normalize(-vec3(pos));\n"
				"  vec3 R = normalize(reflect(-L, N));\n"
				"  vec3 diffuse = Kd * Id * max(dot(L, N), 0.0);\n"
				"  vec3 ambient = Ka * Ia;\n"
				"  vec3 specular = Ks * Is * pow(max(dot(R, V), 0.0), p);\n"
				"  fcolor = diffuse + ambient + specular;\n"
				"  fPosition = pos.xyz;\n"
				"  // we need to put NDC coordinates into gl_Position\n"
				"  gl_Position = projectionMatrix * pos;\n"
				"}";

static const std::string default_fragment_shader =
				"#version 410\n"
				"in vec3 fNormal;\n"
				"in vec3 fPosition;\n"
				"in vec3 fcolor; // getting color date from Vertex shader\n"
				"// We need to create at least one \"out vec4\" for setting color fragment colors\n"
				"out vec4 fragColor; // r,g,b,a\n"
				"void main() {\n"
				"  fragColor = vec4(fcolor, 1.0);\n"
				"}";

#endif /* !COMPUTERGRAPHICS1_DEFAULTSHADER_HPP */
