#version 410

layout (location = 0) in vec3 vertexPosition; // set the attr. number
layout (location = 1) in vec3 vertexNormal;

out vec3 fNormal;
out vec3 fPosition;
out vec3 fcolor; // for sending vertex color to Fragment shader

const mat3 normalMatrix = mat3(1.0);
uniform mat4 modelMatrix; // getting from app (const int shader, you are not allowes to change it)
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

const vec4 lightLocation = vec4(0, 10, 0, 1);
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;

const vec3 Id = vec3(1, 1, 1);
const vec3 Ia = vec3(1, 1, 1);
const vec3 Is = vec3(1, 1, 1);
const float p = 10.0; // phong exponents

void main() {
	fNormal = normalize(normalMatrix * vertexNormal);
  //pos : position in camera coord.
  vec4 pos = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);

  vec3 L = normalize(vec3(lightLocation - pos));
  vec3 N = fNormal;
  vec3 V = normalize(-vec3(pos));
  vec3 R = normalize(reflect(-L, N));

  vec3 diffuse = Kd * Id * max(dot(L, N), 0.0);
  vec3 ambient = Ka * Ia;
  vec3 specular = Ks * Is * pow(max(dot(R, V), 0.0), p);

  fcolor = diffuse + ambient + specular;
  fPosition = pos.xyz;
  // we need to put NDC coordinates into gl_Position
  gl_Position = projectionMatrix * pos;
}