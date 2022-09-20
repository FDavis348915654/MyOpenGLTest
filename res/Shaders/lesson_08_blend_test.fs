#version 330 core
// 混合

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色

void main()
{
	//FragColor = vec4(vec3(texture(texture_diffuse1, TexCoords)), 1.0);
	//FragColor = texture(texture_diffuse1, TexCoords);
	// 采用丢弃片段的方式
	vec4 texColor = texture(texture_diffuse1, TexCoords);
	if (texColor.a < 0.1) {
		discard;
	}
	FragColor = texColor;
}
