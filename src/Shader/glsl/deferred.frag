R"=====(
#version 410

in vec2 uv;

out vec4 fColor;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    float radius;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;

    float cutoff;
    float innerCutoff;
    float exponent;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
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

uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform DirLight directionalLight;

uniform float constant;
uniform float linear;
uniform float quadratic;

subroutine vec3 calcColor_t(ObjectInfos infos);
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
vec3 CalcPointLight(ObjectInfos infos) {
    if (length(pointLight.position - infos.position) > pointLight.radius) discard;

    LightInfos lightInfos = CalcLightInformations(pointLight.position, infos);

    vec3 diffuse = infos.diffuseAlbedo * pointLight.diffuse * lightInfos.dotNL * pointLight.intensity * lightInfos.attenuation;
    vec3 specular = infos.specularAlbedo * pointLight.specular * lightInfos.pf * pointLight.intensity * lightInfos.attenuation;

    return diffuse + specular;
}

subroutine(calcColor_t)
vec3 CalcSpotLight(ObjectInfos infos)
{
    LightInfos lightInfos = CalcLightInformations(spotLight.position, infos);

    float spotDir = dot(-lightInfos.L, normalize(spotLight.direction));
    float angle = acos(spotDir);// radian angle

    float spotAttenuation;
    if (angle < radians(spotLight.innerCutoff)) spotAttenuation = 1.0;
    else {
        float spotValue = smoothstep(cos(radians(spotLight.cutoff)), cos(radians(spotLight.innerCutoff)), spotDir);
        spotAttenuation = pow(spotValue, spotLight.exponent);
    }
    float attenuation = lightInfos.attenuation * spotAttenuation;

    vec3 diffuse = infos.diffuseAlbedo * spotLight.diffuse * lightInfos.dotNL * spotLight.intensity * lightInfos.attenuation;
    vec3 specular = infos.specularAlbedo * spotLight.specular * lightInfos.pf * spotLight.intensity * lightInfos.attenuation;

    return diffuse + specular;
}

subroutine(calcColor_t)
vec3 CalcDirectionalLight(ObjectInfos infos)
{
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 viewDir = normalize(cameraPosition - infos.position);

    // diffuse shading
    float diff = max(dot(infos.normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, infos.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), infos.shiness);

    // combine results
    vec3 diffuse  = infos.diffuseAlbedo * directionalLight.diffuse * diff * directionalLight.intensity;
    vec3 specular = infos.specularAlbedo * directionalLight.specular * spec * directionalLight.intensity;

    return diffuse + specular;
}

subroutine(calcColor_t)
vec3 CalcAmbientColor(ObjectInfos infos) {
    return infos.ambientAlbedo;
}

void main() {
    ObjectInfos infos;
    infos.position = texture(gPosition, uv).rgb;
    infos.normal = texture(gNormal, uv).rgb;
    infos.ambientAlbedo = texture(gAmbientAlbedo, uv).rgb;
    infos.diffuseAlbedo = texture(gDiffuseAlbedo, uv).rgb;
    vec4 spec = texture(gSpecular, uv);
    infos.specularAlbedo = spec.rgb;
    infos.shiness = spec.a;

    if (infos.shiness == 0) discard;

    fColor = vec4(CalcColor(infos), 1.0);
}
)=====";