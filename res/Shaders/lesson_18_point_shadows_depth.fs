#version 330 core
// 点光源阴影

//out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

in vec4 FragPos;

uniform vec3 lightPos;
uniform float near_plane;
uniform float far_plane;

void main()
{
	float lightDistance = length(FragPos.xyz - lightPos);

	// map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

	// write this as modified depth
    gl_FragDepth = lightDistance;
}
