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

const float[9] Sx = float[](
-1.0, 0.0, 1.0,
-2.0, 0.0, 2.0,
-1.0, 0.0, 1.0
);

const float[9] Sy = float[](
-1.0, -2.0, -1.0,
0.0, 0.0, 0.0,
1.0, 2.0, 1.0
);

void main()
{
    float gx = 0;
    float gy = 0;

    for (int i = 0; i < 9; i++) {
        vec4 color = texture(tex, uv + offsets[i]);
        float greyscale = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
        gx += greyscale * Sx[i];
        gy += greyscale * Sy[i];
    }

    float g = sqrt(pow(gx, 2) + pow(gy, 2));
    final_color = vec4(vec3(g), 1.0);
}