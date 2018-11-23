precision highp float;
attribute vec3 position;
attribute vec3 normal;
uniform mat3 normalMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
varying vec3 fNormal;
varying vec3 fPosition;
varying vec3 fcolor;

const vec4 lightLocation = vec4(0, 10, 0, 1);
const vec3 Kd = vec3(1, 1, 0);
const vec3 Id = vec3(1, 1, 1);
const vec3 Ka = vec3(0.2, 0.2, 0);
const vec3 Ia = vec3(1, 1, 1);
const vec3 Ks = vec3(0.3, 0.3, 0);
const vec3 Is = vec3(1, 1, 1);
const float p = 10.0; // phong exponent

void main()
{
  fNormal = normalize(normalMatrix * normal);
  //pos : position in camera coord.
  vec4 pos = modelViewMatrix * vec4(position, 1.0);
  
  vec3 L = normalize(vec3(lightLocation - pos));
  vec3 N = fNormal;
  vec3 V = normalize(-vec3(pos));
  vec3 R = normalize(reflect(-L, N));
  
  vec3 diffuse = Kd * Id * max(dot(L, N), 0.0);
  vec3 ambient = Ka * Ia;
  vec3 specular = Ks * Is * pow(max(dot(R, V), 0.0), p);
  
  fcolor = diffuse + ambient + specular;
  fPosition = pos.xyz;
  gl_Position = projectionMatrix * pos;
}