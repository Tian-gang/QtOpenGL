#version 330 core

out vec4 FragColor;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    float ambinentStrength = 0.1;
    vec3 ambient = ambinentStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objColor;

    FragColor = vec4(result, 1.0);
}
