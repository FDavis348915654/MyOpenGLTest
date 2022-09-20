#version 330 core
// 深度测试

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色

void main()
{
	//vec3 result = vec3(texture(texture_diffuse1, TexCoords));
	vec3 result = vec3(gl_FragCoord.z); // 显示深度值
	//vec3 result = vec3(gl_FragCoord); // 显示深度值
	FragColor = vec4(result, 1.0);
}
