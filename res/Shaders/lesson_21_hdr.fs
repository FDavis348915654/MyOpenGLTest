#version 330 core
// HDR

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	if (hdr) {
		// reinhard
		vec3 result = hdrColor / (hdrColor + vec3(1.0));

		// exposure
		// vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

		// gamma correct
		result = pow(result, vec3(1.0 / gamma));
		FragColor = vec4(result, 1.0);
	}
	else {
		vec3 result = pow(hdrColor, vec3(1.0 / gamma)).rgb;
		FragColor = vec4(result, 1.0);
	}
}
