#version 410

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;

void main()
{
    vec4 color = texture(tex, uv);

    float outputRed = (color.r * .393) + (color.g *.769) + (color.b * .189);
    float outputGreen = (color.r * .349) + (color.g *.686) + (color.b * .168);
    float outputBlue = (color.r * .272) + (color.g *.534) + (color.b * .131);

    final_color = vec4(outputRed, outputGreen, outputBlue, 1.0);
}