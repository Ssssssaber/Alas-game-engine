#shader vertex

#version 410 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_model;

void main() {
    gl_Position = u_viewProjectionMatrix * u_model * vec4(a_Position, 1.0);
}

#shader fragment

#version 330 core

out vec4 FragColor;

uniform vec4 u_Color;

void main() {
    FragColor = u_Color; // Red color
}