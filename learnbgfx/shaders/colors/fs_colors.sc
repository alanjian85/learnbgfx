$input v_position, v_normal, v_texcoord0

#include "common.sh"

uniform vec4 u_lightPosition;
uniform vec4 u_lightDirection;
uniform vec4 u_lightAmbient;
uniform vec4 u_lightDiffuse;
uniform vec4 u_lightSpecular;
uniform vec4 u_lightAttenuation;
uniform vec4 u_lightCones;

SAMPLER2D(s_materialDiffuse, 0);
SAMPLER2D(s_materialSpecular, 1);
uniform vec4 u_materialShininess;

uniform vec4 u_viewPos;

void main() {
    vec3 ambient = vec3(u_lightAmbient) * vec3(texture2D(s_materialDiffuse, v_texcoord0));

    vec3 position = vec3(v_position);
    vec3 lightPos = vec3(u_lightPosition);
    vec3 lightDir = normalize(lightPos - position);
    vec3 normal = normalize(v_normal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(u_lightDiffuse) * diff * vec3(texture2D(s_materialDiffuse, v_texcoord0));

    vec3 viewDir = normalize(vec3(u_viewPos) - v_position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), float(u_materialShininess));
    vec3 specular = vec3(u_lightSpecular) * spec * vec3(texture2D(s_materialSpecular, v_texcoord0));

    float theta = dot(lightDir, normalize(-vec3(u_lightDirection)));
    float epsilon = u_lightCones.x - u_lightCones.y;
    float intensity = clamp((theta - u_lightCones.y) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(lightPos - position);
    float attenuation = 1.0 / (u_lightAttenuation.x + u_lightAttenuation.y * distance + u_lightAttenuation.z * distance * distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}