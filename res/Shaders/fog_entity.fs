#version 330 core
// 迷雾的遮罩, 渲染到一整块迷雾贴图上

in vec2 TexCoords;
out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D diffuse; // 漫反射纹理

void main()
{
	FragColor = texture(diffuse, TexCoords);
}
