#shader vertex

#version 450
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 o_TexCoord;
out flat int o_EntityId;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_model;
uniform int u_EntityId;

void main()
{
    gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position.xy, 0.0, 1.0);	
    
    o_TexCoord = a_TexCoord;
    o_EntityId = u_EntityId;
}

#shader fragment

#version 450

out vec4 color;
out int color2;

in vec2 o_TexCoord;
in flat int o_EntityId;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
    color = texture(u_Texture, o_TexCoord) * u_Color;
    color2 = 50;
}
