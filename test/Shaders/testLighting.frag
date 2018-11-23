precision highp float;
uniform float time;
uniform vec2 resolution;
varying vec3 fPosition;
varying vec3 fNormal;
varying vec3 fcolor;

void main()
{
  gl_FragColor = vec4(fcolor, 1.0);
}