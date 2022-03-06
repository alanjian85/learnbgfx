$input a_position, a_normal
$output v_color0

#include "common.sh"

uniform vec4 u_lightColor;
uniform vec4 u_lightPos;
uniform vec4 u_viewPos;

void main() {
    gl_Position = u_modelViewProj * vec4(a_position, 1.0);
    
    vec3 position = vec3(u_model[0] * vec4(a_position, 1.0));
    vec3 normal = normalize(mat3(transpose(inverse(u_model[0]))) * a_normal);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(u_lightColor);

    vec3 lightPos = vec3(u_lightPos);
    vec3 lightDir = normalize(lightPos - position);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(u_lightColor);

    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(u_viewPos) - position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(u_lightColor);

    v_color0 = ambient + diffuse + specular;
}