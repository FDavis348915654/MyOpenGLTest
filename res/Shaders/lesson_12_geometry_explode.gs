#version 330 core
// 几何着色器

// 内建变量
//in gl_Vertex
//{
//    vec4  gl_Position;
//    float gl_PointSize;
//    float gl_ClipDistance[];
//} gl_in[];

// 这个输入布局修饰符可以从顶点着色器接收下列任何一个图元值：
// points：绘制GL_POINTS图元时（1）。
// lines：绘制GL_LINES或GL_LINE_STRIP时（2）
// lines_adjacency：GL_LINES_ADJACENCY或GL_LINE_STRIP_ADJACENCY（4）
// triangles：GL_TRIANGLES、GL_TRIANGLE_STRIP或GL_TRIANGLE_FAN（3）
// triangles_adjacency：GL_TRIANGLES_ADJACENCY或GL_TRIANGLE_STRIP_ADJACENCY（6）
layout (triangles) in;

// 输出布局修饰符也可以接受几个图元值：
// points
// line_strip
// triangle_strip
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec2 texCoords;
} gs_in[];

out vec2 TexCoords;

uniform float time;

// EmitVertex();
// EndPrimitive();

vec3 GetNormal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
	//float magnitude = 2.0; // 大爆炸
	float magnitude = 0.1; // 小爆炸
	vec3 direction = normal * ((sin(time * 0.3) + 1.0) / 2.0) * magnitude;
	return position + vec4(direction, 0.0);
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	TexCoords = gs_in[0].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	TexCoords = gs_in[1].texCoords;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	TexCoords = gs_in[2].texCoords;
	EmitVertex();
	EndPrimitive();
}
