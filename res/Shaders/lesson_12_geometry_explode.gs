#version 330 core
// 几何着色器 // 搭个小房子

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
	vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position) {
	fColor = gs_in[0].color; // gs_in[0] 因为只有一个输入顶点
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:左下
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0); // 2:右下
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0); // 3:左上
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0); // 4:右上
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0); // 5:顶部
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	build_house(gl_in[0].gl_Position);
}
