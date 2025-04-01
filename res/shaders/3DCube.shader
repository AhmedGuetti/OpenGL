#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 v_Color;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_FragPos = vec3(u_Model * vec4(position, 1.0));
    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
    v_Color = color;
    v_Normal = mat3(transpose(inverse(u_Model))) * normal;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Color;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPosition;

void main()
{
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * u_LightColor;
    
    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_ViewPosition - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor;
    
    // Apply lighting to vertex colors
    vec3 result = (ambient + diffuse + specular) * v_Color;
    
    // Ensure we get some color even if lighting calculation fails
    if (length(result) < 0.1) {
        result = v_Color * 0.5;
    }
    
    color = vec4(result, 1.0);
}