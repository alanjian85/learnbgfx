$input v_position, v_normal

#include "common.sh"

uniform vec4 u_material_ambient;
uniform vec4 u_material_diffuse;
uniform vec4 u_material_specular;
uniform vec4 u_material_shininess;

uniform vec4 u_lightColor;
uniform vec4 u_lightPos;
uniform vec4 u_viewPos;

void main() {
    vec3 ambient = vec3(u_lightColor) * vec3(u_material_ambient);

    vec3 lightPos = vec3(u_lightPos);
    vec3 lightDir = normalize(lightPos - v_position);
    vec3 normal = normalize(v_normal);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(u_lightColor) * (diff * vec3(u_material_diffuse));

    vec3 viewDir = normalize(vec3(u_viewPos) - v_position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), float(u_material_shininess));
    vec3 specular = vec3(u_lightColor) * (spec * vec3(u_material_specular));

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}