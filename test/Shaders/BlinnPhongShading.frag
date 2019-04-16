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

uniform uint lightNumber;
uniform Light lights[MAX_LIGHTS];

uniform uint spotNumber;
uniform SpotLight spotLights[MAX_LIGHTS];

uniform Material material;

const float a = 1.0;
const float b = 0.1;
const float c = 0.01;

uniform vec3 fogColor;
uniform float minDist;
uniform float maxDist;

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

vec3 CalcColor(Light light) {
    LightInfos infos = CalcLightInformations(light.position, fNormal);

    vec3 diffuse = material.diffuse * light.diffuse * infos.dotNL * light.intensity * infos.attenuation;
    vec3 specular = material.specular * light.specular * infos.pf * light.intensity * infos.attenuation;

    return diffuse + specular;
}

vec3 CalcSpotColor(SpotLight light) {
    LightInfos infos = CalcLightInformations(light.position, fNormal);

    float spotDir = dot(-infos.L, normalize(light.direction));
    float angle = acos(spotDir);// radian angle

    float spotAttenuation;
    if (angle < radians(light.innerCutoff)) spotAttenuation = 1.0;
    else {
        float spotValue = smoothstep(cos(radians(light.cutoff)), cos(radians(light.innerCutoff)), spotDir);
        spotAttenuation = pow(spotValue, light.exponent);
    }
    float attenuation = infos.attenuation * spotAttenuation;

    vec3 diffuse = material.diffuse * light.diffuse * light.intensity * infos.dotNL * attenuation;
    vec3 specular = material.specular * light.specular * light.intensity * infos.pf * attenuation;

    return diffuse + specular;
}

void main() {
    vec3 result = material.ambient;
    for (int i = 0; i < lightNumber; i++) {
        result += CalcColor(lights[i]);
    }
    for (int i = 0; i < spotNumber; i++) {
        result += CalcSpotColor(spotLights[i]);
    }

    //float z = length(fPosition.xyz);
    //float f = (maxDist - z) / (maxDist - minDist);
    //f = clamp(f, 0.0, 1.0);
    //vec3 color = mix(fogColor, result, f);

    fragColor = vec4(result, 1);
}
