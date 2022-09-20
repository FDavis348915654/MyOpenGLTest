#version 330 core
// 抗锯齿 的 帧缓冲

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色
uniform int effectType;

// 用于核效果
const float offset = 1.0 / 300.0;

// 获取核效果的颜色
vec3 GetKernelEffect(float[9] kernel) {
	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset), // 左上
		vec2( 0.0f,    offset), // 正上
		vec2( offset,  offset), // 右上
		vec2(-offset,  0.0f),   // 左
		vec2( 0.0f,    0.0f),   // 中
		vec2( offset,  0.0f),   // 右
		vec2(-offset, -offset), // 左下
		vec2( 0.0f,   -offset), // 正下
		vec2( offset, -offset)  // 右下
	);
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(texture_diffuse1, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}
	return col;
}

void main()
{
	if (1 == effectType) { // 反相
		FragColor = vec4(vec3(1.0 - texture(texture_diffuse1, TexCoords)), 1.0);
	}
	else if (2 == effectType) { // 灰度
		FragColor = texture(texture_diffuse1, TexCoords);
		float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
		FragColor = vec4(average, average, average, 1.0);
	}
	else if (3 == effectType) { // 灰度2
		FragColor = texture(texture_diffuse1, TexCoords);
		float average = FragColor.r * 0.2126 + FragColor.g * 0.7152 + FragColor.b * 0.0722;
		FragColor = vec4(average, average, average, 1.0);
	}
	else if (4 == effectType) { // 核效果(锐化)
		float kernel[9] = float[](
			-1, -1, -1,
			-1,  9, -1,
			-1, -1, -1
		);
		FragColor = vec4(GetKernelEffect(kernel), 1.0);
	}
	else if (5 == effectType) { // 核效果(模糊)
		float kernel[9] = float[](
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16
		);
		FragColor = vec4(GetKernelEffect(kernel), 1.0);
	}
	else if (6 == effectType) { // 核效果(边缘检测)
		float kernel[9] = float[](
			1.0, 1.0, 1.0,
			1.0, -8.0, 1.0,
			1.0, 1.0, 1.0
		);
		FragColor = vec4(GetKernelEffect(kernel), 1.0);
	}
	else {
		FragColor = texture(texture_diffuse1, TexCoords);
	}
}
