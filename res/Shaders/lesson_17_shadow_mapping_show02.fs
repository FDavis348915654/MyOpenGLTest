#version 330 core
// 阴影映射(显示调试阴影)

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4
in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
	float depthValue = texture(depthMap, TexCoords).r;
	FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspectiv
}
