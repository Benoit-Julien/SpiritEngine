#version 410

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;

void main()
{
    vec4 color = texture(tex, uv);
    float res = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    final_color = vec4(vec3(res), 1.0);
}