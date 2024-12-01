#version 330 core
// 迷雾(后处理)

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色

void main()
{
	// FragColor = texture(texture_diffuse1, TexCoords); // 正常绘制

	vec4 texColor = texture(texture_diffuse1, TexCoords);
	FragColor = vec4(texColor.rrr, texColor.r); // 正常绘制
}
