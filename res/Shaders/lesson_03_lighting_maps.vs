#version 330 core
// 光照贴图

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// 注意乘法要从右向左读
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	Normal = mat3(transpose(inverse(model))) * aNormal; // 法线矩阵, 法线矩阵被定义为「模型矩阵左上角3x3部分的逆矩阵的转置矩阵」; 这样做消耗比较大, 会计算立方体旋转和缩放后的法线
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
}
