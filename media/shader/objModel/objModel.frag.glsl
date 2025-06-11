#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

uniform sampler2D shadowMap; // 新增：阴影贴图
uniform mat4 lightSpaceMatrix; // 新增：光源空间变换矩阵

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

// 阴影计算函数
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // 透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0, 1]范围
    projCoords = projCoords * 0.5 + 0.5;

    // 如果片元在光源视锥体外，或者在深度贴图边界外，则不计算阴影
    if(projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
    return 0.0; // 0.0 表示完全没有阴影

    float shadow = 0.0;
    // 计算偏置，防止阴影痤疮（shadow acne）
    // 偏置量与法线和光源方向的点积相关，使得平行于光源方向的表面偏置更小
    float bias = max(0.0015 * (1.0 - dot(normal, lightDir)), 0.001);

    // PCF (Percentage-Closer Filtering) 软阴影
    float texelSize = 1.0 / 1024.0; // 假设阴影贴图大小为1024x1024
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            // 从阴影贴图中采样深度值
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            // 比较当前片元的深度和阴影贴图中的深度
            shadow += (projCoords.z - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; // 9个采样点的平均值

    return shadow;
}

void main()
{
    // 获取基础纹理颜色
    vec4 texColor = texture(material.diffuse, TexCoords);

    // 确保法线是单位向量
    vec3 norm = normalize(Normal);
    // 获取观察方向
    vec3 viewDir = normalize(viewPos - FragPos);

    // 如果是水面（法线朝上），添加一些特殊效果
    if (norm.y > 0.9) { // 使用规范化后的法线进行判断
        // 增强波浪效果
        float wave = sin(FragPos.x * 2.0 + time) * 0.5 + 0.5;
        wave *= sin(FragPos.z * 2.0 + time * 1.5) * 0.5 + 0.5;

        // 调整颜色，使水面更蓝
        vec3 waterColor = vec3(0.0, 0.5, 1.0);
        texColor.rgb = mix(texColor.rgb, waterColor, wave * 0.5);

        // 增强高光效果
        float spec = pow(max(dot(norm, viewDir), 0.0), 64.0); // 使用规范化后的法线
        texColor.rgb += vec3(1.0) * spec * 0.8;

        // 添加一些波纹效果
        float ripple = sin(length(FragPos.xz) * 4.0 - time * 2.0) * 0.5 + 0.5;
        texColor.rgb = mix(texColor.rgb, waterColor, ripple * 0.2);
    }

    if(texColor.a < 0.1) {
        // 如果纹理的alpha值很低，则丢弃该片元，模拟透明或镂空效果
        discard;
    }

    // 环境光
    vec3 ambient = ambientIntensity * ambientColor * texColor.rgb;

    // --- 方向光计算 ---
    vec3 lightDir_directional = normalize(directionalLight.direction); // 方向光方向
    // 计算方向光的阴影
    float shadow_directional = ShadowCalculation(lightSpaceMatrix * vec4(FragPos, 1.0), norm, -lightDir_directional); // 注意：这里传入的是光源到片元的方向

    // 漫反射 (方向光)
    float diff_directional = max(dot(norm, -lightDir_directional), 0.0);
    vec3 diffuse_directional = (1.0 - shadow_directional) * diff_directional * directionalLight.intensity * directionalLight.color * texColor.rgb;

    // 镜面反射 (方向光)
    float specularStrength_directional = 0.5;
    vec3 reflectDir_directional = reflect(lightDir_directional, norm); // reflect函数要求L是从光源到点的方向
    float spec_directional = pow(max(dot(viewDir, reflectDir_directional), 0.0), 32.0);
    vec3 specular_directional = (1.0 - shadow_directional) * specularStrength_directional * spec_directional * directionalLight.intensity * directionalLight.color;


    // --- 聚光灯计算 ---
    vec3 spotToFrag = normalize(FragPos - spotLight.position); // 从聚光灯位置到片元的方向
    vec3 spotDir = normalize(spotLight.direction); // 聚光灯方向

    // 聚光灯锥形衰减因子
    float theta = dot(-spotToFrag, spotDir); // 注意：这里是 -spotToFrag，表示从片元到聚光灯的方向
    float epsilon = cos(radians(spotLight.angle)) - cos(radians(spotLight.angle * 0.9)); // 软边缘计算
    float intensity_spotCone = clamp((theta - cos(radians(spotLight.angle))) / epsilon, 0.0, 1.0);

    // 聚光灯距离衰减因子
    float distance_spot = length(spotLight.position - FragPos);
    float attenuation_spot = 1.0 / (spotLight.kc + spotLight.kl * distance_spot + spotLight.kq * distance_spot * distance_spot);

    // 漫反射 (聚光灯)
    vec3 diffuse_spot = max(dot(norm, -spotToFrag), 0.0) * spotLight.intensity * spotLight.color * texColor.rgb;

    // 镜面反射 (聚光灯)
    vec3 reflectDir_spot = reflect(spotToFrag, norm); // reflect函数要求L是从光源到点的方向
    vec3 specular_spot = pow(max(dot(viewDir, reflectDir_spot), 0.0), 32.0) * spotLight.intensity * spotLight.color;

    // 结合聚光灯的锥形和距离衰减
    vec3 spotResult = (diffuse_spot + specular_spot) * intensity_spotCone * attenuation_spot;


    // 合并所有光照效果
    // 注意：聚光灯通常不需要阴影贴图，因为它本身就有一个局部锥形范围，
    // 但如果需要，也可以为聚光灯添加阴影（需要额外一张阴影贴图或复杂的处理）
    // 目前只为方向光添加了阴影
    vec3 resultColor = ambient + diffuse_directional + specular_directional + spotResult;
    FragColor = vec4(resultColor, texColor.a);
}