#version 330 core
out vec4 FragColor;

in vec3 FragPos_worldspace;
in vec3 Normal_worldspace;
in vec2 TexCoords_FS;

uniform sampler2D texture_diffuse1;
// uniform vec3 viewPos;

// uniform vec3 u_objectBaseColor;
// uniform float u_materialShininess;
// uniform vec3 u_specularColor;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords_FS);

    // if(texColor.a < 0.1)
    //     discard;

    float ambientStrength = 0.2;
    // vec3 ambientLightColor = vec3(1.0, 1.0, 1.0);
    vec3 ambient = ambientStrength * texColor.rgb;

    vec3 norm = normalize(Normal_worldspace);
    vec3 lightDir_worldspace = normalize(vec3(0.5, 1.0, 0.7));
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    float diff = max(dot(norm, lightDir_worldspace), 0.0);
    vec3 diffuse = diff * lightColor * texColor.rgb;

    /*
    float specularStrength = 0.5;
    vec3 viewDir_worldspace = normalize(viewPos - FragPos_worldspace);
    vec3 halfwayDir = normalize(lightDir_worldspace + viewDir_worldspace);
    // vec3 reflectDir = reflect(-lightDir_worldspace, norm);
    // float spec = pow(max(dot(viewDir_worldspace, reflectDir), 0.0), u_materialShininess);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), u_materialShininess);
    vec3 specular = specularStrength * spec * lightColor * u_specularColor;
    */

    // vec3 resultColor = ambient + diffuse + specular;
    vec3 resultColor = ambient + diffuse;

    FragColor = vec4(resultColor, texColor.a);

    // FragColor = texColor;
}