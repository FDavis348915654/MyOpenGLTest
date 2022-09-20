#version 330 core
// 抗锯齿

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

void main()
{
	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
