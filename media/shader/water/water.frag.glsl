#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform float time;
uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform vec3 viewPos;

uniform struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
} directionalLight;

uniform struct AmbientLight {
    vec3 color;
    float intensity;
} ambientLight;

void main()
{
    // 为法线贴图（涟漪）创建一个滚动UV，速度可以快一些
    vec2 rippleSpeed = vec2(0.05, 0.03);
    vec2 uv_ripples = TexCoords + time * rippleSpeed;
    vec2 flowSpeed = vec2(0.01, 0.005);
    vec2 uv_flow = TexCoords + time * flowSpeed;

    // --- 法线计算 ---
    vec3 normalFromMap = normalize(texture(normalMap, uv_ripples).rgb * 2.0 - 1.0);
    vec3 finalNormal = normalize(Normal + normalFromMap);

    vec4 baseColor = texture(diffuseTexture, uv_flow);
    vec3 ambient = ambientLight.intensity * ambientLight.color;
    vec3 lightDir = normalize(-directionalLight.direction);
    float diff = max(dot(finalNormal, lightDir), 0.0);
    vec3 diffuse = directionalLight.intensity * directionalLight.color * diff;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, finalNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = 0.8 * spec * directionalLight.color;

    // 组合光照和物体颜色
    float brightness = 1.3;
    vec3 result = (ambient + diffuse) * baseColor.rgb + specular;

    // 设置水的alpha值，让它半透明
    FragColor = vec4(brightness*result, 0.25);
}