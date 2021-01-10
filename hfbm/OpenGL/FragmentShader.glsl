#version 460

in vec3 vs_position;
in vec3 vs_normal;
in vec3 vs_color;

out vec4 fs_color;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

	vec3 posToLightDirVec = normalize(lightPosition - vs_position);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;

	// TODO remove specular light?
	vec3 lightToPosDirVec = normalize(vs_position - lightPosition);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPosition - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = vec3(1.f, 1.f, 1.f) * specularConstant;

	fs_color = vec4(vs_color, 1.f) * (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}