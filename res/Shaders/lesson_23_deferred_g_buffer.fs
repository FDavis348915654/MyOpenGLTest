#version 330 core

// 延迟着色法(记录顶点坐标、法线、漫反射颜色、镜面高光)

// MRT(MultipleRenderTargets)
layout (location = 0) out vec3 gPosition; // 顶点坐标
layout (location = 1) out vec3 gNormal; // 法线坐标
layout (location = 2) out vec4 gAlbedoSpec; // 漫反射颜色+镜面高光

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
    // 保存顶点坐标
    gPosition = FragPos;
    // 法线坐标
    gNormal = normalize(Normal);
    // 漫反射颜色
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // 镜面高光
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;

    //gPosition = vec3(1.0, 0.0, 0.0);
    //gNormal = vec3(1.0, 0.0, 0.0);
    //gAlbedoSpec = vec4(1.0, 0.0, 0.0, 1.0);
}