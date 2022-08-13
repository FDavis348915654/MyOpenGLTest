#version 330 core
// 帧缓冲
layout (location = 0) in vec2 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	// 注意乘法要从右向左读
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	TexCoords = aTexCoords;
}
