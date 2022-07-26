#version 330 core

in vec2 texCoord;

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D texture1; // 采样器(Sampler)
uniform sampler2D texture2; // 采样器(Sampler)

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	//fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2) * vec4(lightColor * objectColor, 1.0);
	fragColor = vec4(lightColor * objectColor, 1.0);
}
