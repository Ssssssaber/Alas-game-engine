#shader vertex 

#version 410 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;


out vec4 o_Color;
out vec2 o_TexCoord;

uniform mat4 u_viewProjectionMatrix;

void main()
{
    gl_Position = u_viewProjectionMatrix * vec4(a_Position, 1.0);	
    
    o_TexCoord = a_TexCoord;
}

#shader fragment

#version 410 core

layout(location = 0) out vec4 color;

in vec4 o_Color;
in vec2 o_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_TilingFactor;

void main()
{
    // color = texture(u_Texture, o_TexCoord) * u_Color;
    color = o_Color;
}
