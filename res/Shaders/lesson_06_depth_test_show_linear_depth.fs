#version 330 core
// 深度测试

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far * 3; // 为了演示除以 far
	vec3 result = vec3(depth); // 显示线性深度值
	FragColor = vec4(result, 1.0);
}
