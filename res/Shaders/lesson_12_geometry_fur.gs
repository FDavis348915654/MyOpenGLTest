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
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
} gs_in[];

// EmitVertex();
// EndPrimitive();

const float MAGNITUDE = 0.4;

uniform mat4 projection;

void GenerateLine(int index) {
	gl_Position = projection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}
