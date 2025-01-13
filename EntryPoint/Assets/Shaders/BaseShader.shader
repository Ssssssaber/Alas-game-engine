#shader vertex core

#version 410
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec3 o_Position;
out vec4 o_Color;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_model;

void main()
{
    o_Position = a_Position;
    o_Color = a_Color;
    gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position, 1.0);	
}

#shader fragment

#version 410 core

layout(location = 0) out vec4 color;

in vec3 o_Position;
in vec4 o_Color;

uniform vec4 u_Color;

void main()
{
    color = u_Color;
}
