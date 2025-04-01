#shader vertex
#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
}

#shader fragment
#version 330 core
uniform vec4 u_Color;
out vec4 FragColor;

void main()
{
    FragColor = u_Color;
}