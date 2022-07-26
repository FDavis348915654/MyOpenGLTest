#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D texture1; // 采样器(Sampler)
uniform sampler2D texture2; // 采样器(Sampler)
uniform float mixValue;

void main()
{
	//fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
	//fragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(1.0 - texCoord.s, texCoord.t)), 0.2); // 修改片段着色器，仅让笑脸图案朝另一个方向看
	//fragColor = texture(texture1, texCoord);
	fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), mixValue);
}
