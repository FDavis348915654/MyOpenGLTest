#version 330 core
// 高级数据/高级GLSL // Uniform缓冲对象

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// gl_VertexID
// gl_FragCoord
// gl_FrontFacing
// gl_FragDepth

void main()
{
	if (gl_FragCoord.x < 400.0) {
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else {
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}
