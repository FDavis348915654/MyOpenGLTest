#version 330 core
// 迷雾灯光

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色

vec3 fogLightColor = vec3(1.0, 1.0, 1.0);

void main()
{
	FragColor = vec4(fogLightColor, texture(texture_diffuse1, TexCoords).a);

	// vec4 textureColor = texture(texture_diffuse1, TexCoords);
	// FragColor = vec4(textureColor.aaa, textureColor.a);

	// FragColor = texture(texture_diffuse1, TexCoords);
}
