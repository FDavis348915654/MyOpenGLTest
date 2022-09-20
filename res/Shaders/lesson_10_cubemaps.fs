#version 330 core
// 立方体贴图
in vec3 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, TexCoords);
}
