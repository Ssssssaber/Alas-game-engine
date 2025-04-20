#shader vertex core

#version 410 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 o_TexCoord;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_model;

void main()
{
    gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position, 1.0);	
    
    o_TexCoord = a_TexCoord;
}

#shader fragment

#version 410 core

out vec4 color;

in vec2 o_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
    color = texture(u_Texture, o_TexCoord) * u_Color;
}
