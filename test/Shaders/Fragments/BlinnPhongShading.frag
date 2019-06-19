#version 410
#define MAX_LIGHTS 32

struct SpotLight {
    float cutoff;
    float innerCutoff;
    float exponent;
    vec3 direction;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;

    bool isSpotLight;
    SpotLight spot;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

struct LightInfos {
    vec3 L;
    float d;
    float attenuation;

    vec3 N;
    vec3 V;
    vec3 H;

    float dotNL;
    float dotHN;
    float pf;
};

in vec3 fNormal;
in vec3 fPosition;
in vec4 fragPosLightSpace;

uniform uint lightNumber;
uniform Light lights[MAX_LIGHTS];

uniform Material material;

const float a = 1.0;
const float b = 0.1;
const float c = 0.01;

const float bias = 0.005;

uniform vec3 fogColor;
uniform float minDist;
uniform float maxDist;

uniform sampler2D depth;

// We need to create at least one "out vec4" for setting color fragment colors
out vec4 fragColor;// r,g,b,a

float CalcAttenuation(float distance) {
    float d = distance;
    float d2 = d * d;
    return 1.0 / (a + b*d + c*d2);
}

LightInfos CalcLightInformations(vec3 lightPosition, vec3 normal) {
    LightInfos infos;

    infos.L = lightPosition - fPosition;
    infos.d = length(infos.L);
    infos.attenuation = CalcAttenuation(infos.d);

    infos.L = normalize(infos.L);
    infos.N = normal;
    infos.V = normalize(-vec3(fPosition));
    infos.H = normalize(infos.V + infos.L);

    infos.dotNL = max(dot(infos.N, infos.L), 0);
    infos.dotHN = max(dot(infos.H, infos.N), 0);
    infos.pf = pow(infos.dotHN, material.shiness);

    return infos;
}

float ShadowCalculation(vec4 PInLightSpace)
{
    // perform perspective divide
    vec3 projCoords = PInLightSpace.xyz / PInLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;  //why? because it has a values [-1, 1], convert to [0, 1]
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Supress the limit of the far / close plane
    if (projCoords.z > 1.0)
        return 0.0;

    vec2 texelSize = 1.0 / textureSize(depth, 0);
    float shadow = 0.0;

    for (int i = 0; i < 9; i++) {
        float x = i % 3 - 1;
        float y = i / 3 - 1;

        float pcfDepth = texture(depth, projCoords.xy + vec2(x, y) * texelSize).r;
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
    }
    shadow /= 9.0f;
    return shadow;
}

vec3 CalcPointLight(Light light) {
    LightInfos infos = CalcLightInformations(light.position, fNormal);

    vec3 diffuse = material.diffuse * light.diffuse * infos.dotNL * light.intensity * infos.attenuation;
    vec3 specular = material.specular * light.specular * infos.pf * light.intensity * infos.attenuation;

    return diffuse + specular;
}

vec3 CalcSpotLight(Light light) {
    LightInfos infos = CalcLightInformations(light.position, fNormal);

    float spotDir = dot(-infos.L, normalize(light.spot.direction));
    float angle = acos(spotDir);// radian angle

    float spotAttenuation;
    if (angle < radians(light.spot.innerCutoff)) spotAttenuation = 1.0;
    else {
        float spotValue = smoothstep(cos(radians(light.spot.cutoff)), cos(radians(light.spot.innerCutoff)), spotDir);
        spotAttenuation = pow(spotValue, light.spot.exponent);
    }
    float attenuation = infos.attenuation * spotAttenuation;

    vec3 diffuse = material.diffuse * light.diffuse * light.intensity * infos.dotNL * attenuation;
    vec3 specular = material.specular * light.specular * light.intensity * infos.pf * attenuation;

    return diffuse + specular;
}

void main() {
    vec3 result = material.ambient;
    float shadow = ShadowCalculation(fragPosLightSpace);

    for (int i = 0; i < lightNumber; i++) {
        Light light = lights[i];
        result += (1 - shadow) * (light.isSpotLight ? CalcSpotLight(light) : CalcPointLight(light));
    }

    //float z = length(fPosition.xyz);
    //float f = (maxDist - z) / (maxDist - minDist);
    //f = clamp(f, 0.0, 1.0);
    //vec3 color = mix(fogColor, result, f);

    fragColor = vec4(result, 1);
}
