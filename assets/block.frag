#version 330 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D dirtTexture;

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

    vec3 phong = (ambient + diffuse + specular);

    fragColor = vec4(phong, 1.0) * texture(dirtTexture, texCoord);
}