#version 330 core

// 基础光照 // 练习题四, 尝试实现一个Gouraud着色（而不是冯氏着色）。如果你做对了话，立方体的光照应该会看起来有些奇怪，尝试推理为什么它会看起来这么奇怪

in vec3 resultColor;

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

void main()
{
	fragColor = vec4(resultColor, 1.0f);
}
