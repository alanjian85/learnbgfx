$input v_position, v_normal, v_texcoord0

#include "common.sh"

uniform vec4 u_light_position;
uniform vec4 u_light_ambient;
uniform vec4 u_light_diffuse;
uniform vec4 u_light_specular;

SAMPLER2D(s_material_diffuse, 0);
SAMPLER2D(s_material_specular, 1);
uniform vec4 u_material_shininess;

uniform vec4 u_lightPos;
uniform vec4 u_viewPos;

void main() {
    vec3 ambient = vec3(u_light_ambient) * vec3(texture2D(s_material_diffuse, v_texcoord0));

    vec3 lightPos = vec3(u_light_position);
    vec3 lightDir = normalize(lightPos - v_position);
    vec3 normal = normalize(v_normal);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(u_light_diffuse) * diff * vec3(texture2D(s_material_diffuse, v_texcoord0));

    vec3 viewDir = normalize(vec3(u_viewPos) - v_position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), float(u_material_shininess));
    vec3 specular = vec3(u_light_specular) * spec * vec3(1.0 - texture2D(s_material_specular, v_texcoord0));

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}