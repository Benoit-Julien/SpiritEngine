#version 410

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;

const float f = 1.0 / 300;

const vec2 offsets[9] = vec2[](
    vec2(-f, f),
    vec2(0, f),
    vec2(f, f),
    vec2(-f, 0),
    vec2(0, 0),
    vec2(f, 0),
    vec2(-f, -f),
    vec2(0, -f),
    vec2(f, -f)
);

const float kernel[9] = float[](
    1.0, 2.0, 1.0,
    2.0, 4.0, 2.0,
    1.0, 2.0, 1.0
);

void main()
{
    vec4 result;

    for (int i = 0; i < 9; i++)
        result += texture(tex, uv + offsets[i]) * kernel[i] / 16;
    final_color = result;
}