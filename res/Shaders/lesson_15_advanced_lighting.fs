#version 330 core
// 高级光照

in VS_OUT {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

void main()
{
	vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
	// ambient, 环境光
	vec3 ambient = 0.05 * color;
	// diffuse, 漫反射
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular, 镜面光
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0;
	if (blinn) {
		float shininess = 16.0; //0.5; //32.0;
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	}
	else {
		float shininess = 4; //0.5; //8.0;
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
	float bright = 0.5; //0.3;
	vec3 specular = vec3(bright) * spec; // assuming bright white light color
	FragColor = vec4(ambient + diffuse + specular, 1.0);
	//FragColor = vec4(ambient + diffuse, 1.0);
	//FragColor = vec4(specular, 1.0);
}
