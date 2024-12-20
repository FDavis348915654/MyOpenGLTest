#version 330 core

// 延迟着色法

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D fboAttachment;

void main()
{
    FragColor = vec4(texture(fboAttachment, TexCoords).rgb, 1.0);
}
