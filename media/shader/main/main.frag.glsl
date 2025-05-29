#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec3 fColor;
in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float ns;
};

struct AmbientLight {
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    vec3 direction;
    float intensity;
    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float intensity;
    vec3 color;
    float angle;
    float kc;
    float kl;
    float kq;
};

uniform Material material;
uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform vec3 cameraPosition;
uniform sampler2D texture_sampler;

// 阴影采样函数（带简单PCF软阴影）
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // 透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 转换到[0,1]区间
    projCoords = projCoords * 0.5 + 0.5;

    // 超出阴影贴图范围则不在阴影中
    if(projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    float shadow = 0.0;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF采样
    float texelSize = 1.0 / 1024.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (projCoords.z - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec3 calcDirectionalLightdiff(vec3 normal, vec3 objectColor) {
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 diffuse = directionalLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;
    return directionalLight.intensity * diffuse * objectColor;
}

vec3 calcSpotLightdiff(vec3 normal, vec3 objectColor) {
    vec3 lightDir = normalize(spotLight.position - fPosition);
    float theta = acos(-dot(lightDir, normalize(spotLight.direction)));
    if (theta > spotLight.angle) {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    vec3 diffuse = spotLight.color * max(dot(lightDir, normal), 0.0f) * material.kd;
    float distance = length(spotLight.position - fPosition);
    float attenuation = 1.0f / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);
    return spotLight.intensity * attenuation * diffuse * objectColor;
}

vec3 calcDirectionalSpecular(vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);
    vec3 specular = material.ks * spec * directionalLight.color;
    return directionalLight.intensity * specular;
}

vec3 calcSpotSpecular(vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(spotLight.position - fPosition);
    float distance = length(spotLight.position - fPosition);
    float attenuation = 1.0 / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);
    float theta = acos(-dot(lightDir, normalize(spotLight.direction)));
    if (theta > spotLight.angle) { 
        return vec3(0.0);
    }
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.ns);
    vec3 specular = material.ks * spec * spotLight.color;
    return spotLight.intensity * attenuation * specular;
};

void main() {
    vec4 texColor = texture(texture_sampler, fTexCoord);
    vec3 normal = normalize(fNormal);
    vec3 viewDir = normalize(cameraPosition - fPosition);

    vec3 ambient = material.ka * ambientLight.color * ambientLight.intensity * texColor.rgb;

    // 阴影判断
    vec3 lightDir = normalize(-directionalLight.direction);
    float shadow = ShadowCalculation(lightSpaceMatrix * vec4(fPosition, 1.0), normal, lightDir);

    vec3 diffuse = calcDirectionalLightdiff(normal, texColor.rgb) + calcSpotLightdiff(normal, texColor.rgb);
    vec3 specular = calcDirectionalSpecular(normal, viewDir) + calcSpotSpecular(normal, viewDir);

    if (texColor.a < 0.1)
    {
        discard;
    }

    // 只对平行光部分做阴影衰减
    vec3 color = ambient +
                 (1.0 - shadow) * calcDirectionalLightdiff(normal, texColor.rgb) +
                 calcSpotLightdiff(normal, texColor.rgb) +
                 (1.0 - shadow) * calcDirectionalSpecular(normal, viewDir) +
                 calcSpotSpecular(normal, viewDir);

    FragColor = vec4(color, texColor.a);
}
