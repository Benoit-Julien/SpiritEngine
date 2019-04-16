#version 410
#define MAX_LIGHTS 32
#define MAX_TEXTURES 16

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

    uint textNumber;
    sampler2D textures[MAX_TEXTURES];
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
in vec2 texCoord;

uniform uint lightNumber;
uniform Light lights[MAX_LIGHTS];

uniform uint spotNumber;
uniform SpotLight spotLights[MAX_LIGHTS];

uniform Material material;

const float a = 1.0;
const float b = 0.1;
const float c = 0.01;

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

vec4 CalcColor(Light light, vec4 texColor) {
    LightInfos infos = CalcLightInformations(light.position, fNormal);

    //vec3 ambient = material.ambient * light.ambient * light.intensity * attenuation;
    vec3 diffuse = material.diffuse * light.diffuse * infos.dotNL * light.intensity * infos.attenuation;
    vec3 specular = material.specular * light.specular * infos.pf * light.intensity * infos.attenuation;

    return (vec4(diffuse, 1.0) * texColor) + vec4(specular, 1.0);
}

vec4 CalcSpotColor(SpotLight light, vec4 texColor) {
    LightInfos infos = CalcLightInformations(light.position, fNormal);

    float spotDir = dot(-infos.L, normalize(light.direction));
    float angle = acos(spotDir);// radian angle

    float spotAttenuation;
    if (angle < radians(light.innerCutoff))
    spotAttenuation = 1.0;
    else {
        float spotValue = smoothstep(cos(radians(light.cutoff)), cos(radians(light.innerCutoff)), spotDir);
        spotAttenuation = pow(spotValue, light.exponent);
    }
    float attenuation = infos.attenuation * spotAttenuation;

    //vec3 ambient = material.ambient * light.ambient * light.intensity * attenuation;
    vec3 diffuse = material.diffuse * light.diffuse * light.intensity * infos.dotNL * attenuation;
    vec3 specular = material.specular * light.specular * light.intensity * infos.pf * attenuation;

    return (vec4(diffuse, 1.0) * texColor) + vec4(specular, 1.0);
}

vec4 CalcTextureColor() {
    vec4 result = texture(material.textures[0], texCoord);
    for (int i = 1; i < material.textNumber; i++) {
        vec4 texColor = texture(material.textures[i], texCoord);
        result = mix(texColor, result, texColor.a);
    }
    return result;
}

void main() {
    vec4 texColor = CalcTextureColor();
    if (texColor.a < 0.1)
        discard;

    vec4 result = vec4(material.ambient, 1.0) * texColor;

    for (int i = 0; i < lightNumber; i++)
        result += CalcColor(lights[i], texColor);
    for (int i = 0; i < spotNumber; i++)
        result += CalcSpotColor(spotLights[i], texColor);

    fragColor = result;
}
