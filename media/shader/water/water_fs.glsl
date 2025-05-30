// water_fs.glsl 准备之后实现水的流动渲染
#version 330 core
out vec4 FragColor;

in vec3 FragPos_worldspace;
in vec3 Normal_worldspace; // 原始几何体法线
in vec2 TexCoords_FS;      // 原始UV

uniform sampler2D normalMap1;       // 法线贴图1
uniform sampler2D normalMap2;       // 法线贴图2 (可选)
uniform float time;               // 时间，从C++传入
uniform vec4 waterColor = vec4(0.2, 0.5, 0.8, 0.7);
uniform vec3 lightDir_worldspace = normalize(vec3(0.5, 1.0, 0.7)); // 光源方向
uniform vec3 viewPos_worldspace;  // 相机位置

void main() {
    // UV滚动
    vec2 scrolledUV1 = TexCoords_FS + vec2(time * 0.02, time * 0.01);
    vec2 scrolledUV2 = TexCoords_FS + vec2(time * -0.015, time * 0.025);

    // 从法线贴图采样并转换到世界空间 (这里简化了TBN矩阵的计算，实际应用可能需要)
    // 假设法线贴图中的法线是切线空间的
    vec3 normalFromMap1 = texture(normalMap1, scrolledUV1).rgb * 2.0 - 1.0;
    vec3 normalFromMap2 = texture(normalMap2, scrolledUV2).rgb * 2.0 - 1.0;
    vec3 perturbedNormal = normalize(Normal_worldspace + normalFromMap1 * 0.3 + normalFromMap2 * 0.3); // 混合并调整强度

    // 简单的光照 (基于扰动后的法线)
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * waterColor.rgb;

    float diff = max(dot(perturbedNormal, lightDir_worldspace), 0.0);
    vec3 diffuse = diff * vec3(1.0); // 白色光源

    // 简单的镜面高光
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos_worldspace - FragPos_worldspace);
    vec3 reflectDir = reflect(-lightDir_worldspace, perturbedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * vec3(1.0); // 白色高光

    vec3 lighting = ambient + diffuse + specular;
    FragColor = vec4(lighting * waterColor.rgb, waterColor.a); // 将光照作用于水体颜色
}