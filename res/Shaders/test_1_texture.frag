#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D ourTexture; // 采样器(Sampler)

void main()
{
	//fragColor = vec4(1.0, 1.0, 1.0, 1.0);
	//fragColor = texture(ourTexture, texCoord)
	fragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
}
