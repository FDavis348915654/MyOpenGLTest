#version 330 core
// 实例化

layout (location = 0) in vec2 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main()
{
	// 加一个缩放效果
	vec2 pos = aPos * (gl_InstanceID / 100.0);
	gl_Position = vec4(pos + aOffset, 0.0, 1.0);
	
	// 正常效果
	//gl_Position = vec4(aPos + aOffset, 0.0, 1.0);

	fColor = aColor;
}
