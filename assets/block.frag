#version 330 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D dirtTexture;

struct Flashlight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    bool on;

    // attenuation
    float constant;
    float linear;
    float quadratic;
};

uniform Flashlight flashlight;

void main()
{
    vec3 lightColor = vec3(1.0); // this should be a uniform, don't hardcode this
    float ambientStrength = 0.1;

    vec3 ambient = ambientStrength * lightColor;

    // diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ambient + diffuse + specular;

    if (flashlight.on) {
        float distance    = length(flashlight.position - fragPos);
        float attenuation = 1.0 / (flashlight.constant + flashlight.linear * distance +
        flashlight.quadratic * (distance * distance));

        vec3 spotDir = normalize(flashlight.position - fragPos);
        float theta = dot(spotDir, normalize(-flashlight.direction));
        float epsilon = flashlight.cutOff - flashlight.outerCutOff;
        float intensity = clamp((theta - flashlight.outerCutOff) / epsilon, 0.0, 1.0);

        float fDiff = max(dot(norm, spotDir), 0.0);
        vec3 fReflect = reflect(-spotDir, norm);
        float fSpec = pow(max(dot(viewDir, fReflect), 0.0), 32);

        result += (fDiff * lightColor + specularStrength * fSpec * lightColor) * intensity * attenuation;
    }

    fragColor = vec4(result, 1.0) * texture(dirtTexture, texCoord);
}