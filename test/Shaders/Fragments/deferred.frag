#version 410

in vec2 uv;

out vec4 fColor;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float radius;
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

struct ObjectInfos {
    vec3 position;
    vec3 normal;
    vec3 ambientAlbedo;
    vec3 diffuseAlbedo;
    vec3 specularAlbedo;

    float shiness;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAmbientAlbedo;
uniform sampler2D gDiffuseAlbedo;
uniform sampler2D gSpecular;

uniform vec3 cameraPosition;

uniform Light light;

uniform float constant;
uniform float linear;
uniform float quadratic;

subroutine vec3 calcColor_t(Light light, ObjectInfos infos);
subroutine uniform calcColor_t CalcColor;

float CalcAttenuation(float distance) {
    float d = distance;
    float d2 = d * d;
    return 1.0 / (constant + linear * d + quadratic * d2);
}

LightInfos CalcLightInformations(vec3 lightPosition, ObjectInfos infos) {
    LightInfos lightInfos;

    lightInfos.L = lightPosition - infos.position;
    lightInfos.d = length(lightInfos.L);
    lightInfos.attenuation = CalcAttenuation(lightInfos.d);

    lightInfos.L = normalize(lightInfos.L);
    lightInfos.N = infos.normal;
    lightInfos.V = normalize(cameraPosition - infos.position);
    lightInfos.H = normalize(lightInfos.V + lightInfos.L);

    lightInfos.dotNL = max(dot(lightInfos.N, lightInfos.L), 0);
    lightInfos.dotHN = max(dot(lightInfos.H, lightInfos.N), 0);
    lightInfos.pf = pow(lightInfos.dotHN, infos.shiness);

    return lightInfos;
}

subroutine(calcColor_t)
vec3 CalcPointLight(Light light, ObjectInfos infos) {
    ObjectInfos infos;
    infos.position = texture(gPosition, uv).rgb;
    infos.normal = texture(gNormal, uv).rgb;
    infos.ambientAlbedo = texture(gAmbientAlbedo, uv).rgb;
    infos.diffuseAlbedo = texture(gDiffuseAlbedo, uv).rgb;
    vec4 spec = texture(gSpecular, uv);
    infos.specularAlbedo = spec.rgb;
    infos.shiness = spec.a;

    if (infos.shiness == 0f)
        discard;

    if (length(light.position - infos.position) > light.radius)
        discard;

    LightInfos lightInfos = CalcLightInformations(light.position, infos);

    vec3 diffuse = infos.diffuseAlbedo * light.diffuse * lightInfos.dotNL * light.intensity * lightInfos.attenuation;
    vec3 specular = infos.specularAlbedo * light.specular * lightInfos.pf * light.intensity * lightInfos.attenuation;

    return diffuse + specular;
}

subroutine(calcColor_t)
vec3 CalcAmbientColor(Light light, ObjectInfos infos) {
    infos.ambientAlbedo = texture(gAmbientAlbedo, uv).rgb;

    if (infos.shiness == 0f)
        discard;

    return infos.ambientAlbedo;
}

void main() {
    fColor = vec4(CalcColor(light, infos), 1.0);
}