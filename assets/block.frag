#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{

    float ambientStrength = 0.1;
    vec3 lightColor = vec3(1.0);
    vec3 ambient = ambientStrength * lightColor;

    FragColor = vec4(ambient, 1.0) * texture(ourTexture, texCoord);
}