#version 330 core

// 延迟着色法(绘制光源)
layout (location = 0) out vec4 FragColor;

uniform vec3 lightColor;

void main() {
    FragColor = vec4(lightColor, 1.0);
}
