#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

struct Material {
    sampler2D diffuse;
    sampler2D normal;
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float angle;
    float kc;
    float kl;
    float kq;
};

uniform Material material;
uniform vec3 viewPos;
uniform float time;

// 光照参数
uniform vec3 ambientColor;
uniform float ambientIntensity;
uniform Light directionalLight;
uniform Light spotLight;

// uniform vec3 u_objectBaseColor;
// uniform float u_materialShininess;
// uniform vec3 u_specularColor;

void main()
{
    // 获取基础纹理颜色
    vec4 texColor = texture(material.diffuse, TexCoords);

    // 如果是水面（法线朝上），添加一些特殊效果
    if (Normal.y > 0.9) {
        // 增强波浪效果
        float wave = sin(FragPos.x * 2.0 + time) * 0.5 + 0.5;
        wave *= sin(FragPos.z * 2.0 + time * 1.5) * 0.5 + 0.5;

        // 调整颜色，使水面更蓝
        vec3 waterColor = vec3(0.0, 0.5, 1.0);
        texColor.rgb = mix(texColor.rgb, waterColor, wave * 0.5);

        // 增强高光效果
        vec3 viewDir = normalize(viewPos - FragPos);
        float spec = pow(max(dot(Normal, viewDir), 0.0), 64.0);
        texColor.rgb += vec3(1.0) * spec * 0.8;

        // 添加一些波纹效果
        float ripple = sin(length(FragPos.xz) * 4.0 - time * 2.0) * 0.5 + 0.5;
        texColor.rgb = mix(texColor.rgb, waterColor, ripple * 0.2);
    }

    if(texColor.a < 0.1) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }

    // 环境光
    vec3 ambient = ambientIntensity * ambientColor * texColor.rgb;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(directionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * directionalLight.intensity * directionalLight.color * texColor.rgb;

    // 镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * directionalLight.intensity * directionalLight.color;

    // 聚光灯
    vec3 spotDir = normalize(spotLight.direction);
    vec3 spotToFrag = normalize(FragPos - spotLight.position);
    float theta = dot(spotToFrag, spotDir);
    float epsilon = cos(radians(spotLight.angle)) - cos(radians(spotLight.angle * 0.9));
    float intensity = clamp((theta - cos(radians(spotLight.angle))) / epsilon, 0.0, 1.0);

    float distance = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.kc + spotLight.kl * distance + spotLight.kq * distance * distance);

    vec3 spotDiffuse = max(dot(norm, -spotToFrag), 0.0) * spotLight.intensity * spotLight.color * texColor.rgb;
    vec3 spotSpecular = pow(max(dot(viewDir, reflect(spotToFrag, norm)), 0.0), 32.0) * spotLight.intensity * spotLight.color;

    vec3 spotResult = (spotDiffuse + spotSpecular) * intensity * attenuation;

    // 合并所有光照效果
    vec3 resultColor = ambient + diffuse + specular + spotResult;
    FragColor = vec4(resultColor, 1.0);

    // FragColor = texColor;
}