#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec3 fColor;
in vec2 fTexCoord;

out vec4 FragColor;

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
}


void main() {
    vec4 texColor = texture(texture_sampler, fTexCoord);
    vec3 normal = normalize(fNormal);
    vec3 viewDir = normalize(cameraPosition - fPosition);

    vec3 ambient = material.ka * ambientLight.color * ambientLight.intensity * texColor.rgb;

    vec3 diffuse = calcDirectionalLightdiff(normal, texColor.rgb) + calcSpotLightdiff(normal, texColor.rgb);
    vec3 specular = calcDirectionalSpecular(normal, viewDir) + calcSpotSpecular(normal, viewDir);

    if (texColor.a < 0.1)
    {
        discard;
    }

    FragColor = vec4((ambient + diffuse + specular), texColor.a);
}