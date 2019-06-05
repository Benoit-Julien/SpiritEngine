R"=====(
#version 410

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;
uniform bool depth;
uniform float zNear;
uniform float zFar;

float LinearizeDepth(in vec2 uv)
{
    float depth = texture(tex, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
    //rrra because depth textures are not usual textures, they have only one channel
    final_color = (depth) ? texture(tex, uv).rrra : texture(tex, uv).rgba;

    float d;
    if (depth) {
        d = LinearizeDepth(uv);
        final_color = vec4(d, d, d, 1.0);
    }
}
)=====";