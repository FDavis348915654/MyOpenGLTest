#version 330 core
// 几何着色器

out vec4 FragColor;

in vec3 fColor;

void main()
{
	FragColor = vec4(fColor, 1.0);
}
