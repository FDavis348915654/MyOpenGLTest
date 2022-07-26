#version 330 core

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform vec3 lightColor;

void main()
{
	fragColor = vec4(lightColor, 1.0);
}
