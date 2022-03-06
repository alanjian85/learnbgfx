$input v_position, v_normal

#include "common.sh"

uniform vec4 u_objectColor;
uniform vec4 u_lightColor;
uniform vec4 u_lightPos;
uniform vec4 u_viewPos;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(u_objectColor);

    vec3 normal = normalize(v_normal);
    vec3 lightDir = normalize(vec3(u_lightPos) - v_position);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(u_lightColor);

    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(u_viewPos) - v_position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(u_lightColor);

    vec3 result = (ambient + diffuse + specular) * vec3(u_objectColor);
    gl_FragColor = vec4(result, 1.0);
}