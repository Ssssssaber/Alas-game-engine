#shader vertex

#version 410 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 o_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_ViewProjectionMatrix;

void main()
{
    o_TexCoord = vertex.zw;
    gl_Position = u_ViewProjectionMatrix * vec4(vertex.xy, 0.0, 1.0);
}

#shader fragment

#version 410 core
in vec2 o_TexCoord;

out vec4 color;

uniform sampler2D u_Text;
uniform vec4 u_Color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, o_TexCoord).r);
    color = u_Color * sampled;
}