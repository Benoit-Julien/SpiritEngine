#version 410
#define MAX_LIGHTS 32

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

struct SpotLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;

    float cutoff;
    float innerCutoff;
    float exponent;
    vec3 direction;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

in vec3 fNormal;
in vec3 fPosition;

uniform uint lightNumber;
uniform Light lights[MAX_LIGHTS];

uniform uint spotNumber;
uniform SpotLight spotLights[MAX_LIGHTS];

uniform Material material;

const float a = 1.0;
const float b = 0.1;
const float c = 0.01;

const int levels = 3;
const float scaleFactor = 1.0 / levels;

// We need to create at least one "out vec4" for setting color fragment colors
out vec4 fragColor;// r,g,b,a

float CalcAttenuation(float distance) {
    float d = distance;
    float d2 = d * d;
    return 1.0 / (a + b*d + c*d2);
}

vec3 CalcColor(Light light) {
    vec3 L = light.position - fPosition;
    float d = length(L);
    float attenuation = CalcAttenuation(d);

    L = normalize(L);
    vec3 N = fNormal;
    vec3 V = normalize(-vec3(fPosition));
    vec3 H = normalize(V + L);

    float dotNL = max(dot(N, L), 0);
    float dotHN = max(dot(H, N), 0);

    float cosine = dot(L, N);
    float value = floor(cosine * levels) * scaleFactor;

    vec3 diffuse = material.diffuse * light.diffuse * dotNL * light.intensity * attenuation;
    diffuse *= value;

    return diffuse;
}

vec3 CalcSpotColor(SpotLight light) {
    vec3 L = light.position - fPosition;
    float d = length(L);
    float attenuation = CalcAttenuation(d);

    L = normalize(L);
    vec3 N = fNormal;
    vec3 V = normalize(-vec3(fPosition));
    vec3 H = normalize(V + L);

    float dotNL = max(dot(N, L), 0);
    float dotHN = max(dot(H, N), 0);

    float cosine = dot(L, N);
    float value = floor(cosine * levels) * scaleFactor;

    float spotDir = dot(-L, normalize(light.direction));
    float angle = acos(spotDir);// radian angle

    float spotAttenuation;
    if (angle < radians(light.innerCutoff))
    spotAttenuation = 1.0;
    else {
        float spotValue = smoothstep(cos(radians(light.cutoff)), cos(radians(light.innerCutoff)), spotDir);
        spotAttenuation = pow(spotValue, light.exponent);
    }
    attenuation *= spotAttenuation;

    vec3 diffuse = material.diffuse * light.diffuse * light.intensity * dotNL * attenuation;
    diffuse *= value;

    return diffuse;
}

void main() {
    vec3 result = material.ambient;
    for (int i = 0; i < lightNumber; i++)
    result += CalcColor(lights[i]);
    for (int i = 0; i < spotNumber; i++)
    result += CalcSpotColor(spotLights[i]);
    fragColor = vec4(result, 1);
}
