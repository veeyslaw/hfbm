#version 460

in vec3 vs_position;
in vec3 vs_normal;
in vec3 vs_color;

out vec4 fs_color;

uniform vec3 lightPos0;

void main()
{
	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;

	fs_color = vec4(vs_color, 1.f) * (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f));
}