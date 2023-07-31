// Light Cube 

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

uniform vec3 u_Color;


void main()
{
    FragColor = vec4(1.0) * vec4(u_Color.xyz, 1.0);
}