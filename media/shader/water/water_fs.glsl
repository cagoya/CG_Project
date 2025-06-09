// water_fs.glsl ׼��֮��ʵ��ˮ��������Ⱦ
#version 330 core
out vec4 FragColor;

in vec3 FragPos_worldspace;
in vec3 Normal_worldspace; // ԭʼ�����巨��
in vec2 TexCoords_FS;      // ԭʼUV

uniform sampler2D normalMap1;       // ������ͼ1
uniform sampler2D normalMap2;       // ������ͼ2 (��ѡ)
//uniform float time;               // ʱ�䣬��C++����
uniform vec4 waterColor = vec4(0.2, 0.5, 0.8, 0.7);
uniform vec3 lightDir_worldspace = normalize(vec3(0.5, 1.0, 0.7)); // ��Դ����
uniform vec3 viewPos_worldspace;  // ���λ��

void main() {
    // UV����
    vec2 scrolledUV1 = TexCoords_FS + vec2(time * 0.02, time * 0.01);
    vec2 scrolledUV2 = TexCoords_FS + vec2(time * -0.015, time * 0.025);

    // �ӷ�����ͼ������ת��������ռ� (�������TBN����ļ��㣬ʵ��Ӧ�ÿ�����Ҫ)
    // ���跨����ͼ�еķ��������߿ռ��
    vec3 normalFromMap1 = texture(normalMap1, scrolledUV1).rgb * 2.0 - 1.0;
    vec3 normalFromMap2 = texture(normalMap2, scrolledUV2).rgb * 2.0 - 1.0;
    vec3 perturbedNormal = normalize(Normal_worldspace + normalFromMap1 * 0.3 + normalFromMap2 * 0.3); // ��ϲ�����ǿ��

    // �򵥵Ĺ��� (�����Ŷ���ķ���)
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * waterColor.rgb;

    float diff = max(dot(perturbedNormal, lightDir_worldspace), 0.0);
    vec3 diffuse = diff * vec3(1.0); // ��ɫ��Դ

    // �򵥵ľ���߹�
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos_worldspace - FragPos_worldspace);
    vec3 reflectDir = reflect(-lightDir_worldspace, perturbedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * vec3(1.0); // ��ɫ�߹�

    vec3 lighting = ambient + diffuse + specular;
    FragColor = vec4(lighting * waterColor.rgb, waterColor.a); // ������������ˮ����ɫ
}