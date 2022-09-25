#version 330 core
// 阴影映射(显示调试阴影)

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4
in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{
	float depthValue = texture(depthMap, TexCoords).r;
	FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
