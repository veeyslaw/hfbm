#version 460

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec3 vertex_color;
layout (location = 3) in vec2 vertex_texcoord;

out vec3 vs_position;
out vec3 vs_normal;
out vec3 vs_color;
out vec2 vs_texcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_position = vec4(ModelMatrix * vec4(vertex_position, 1)).xyz;
	vs_normal = mat3(ModelMatrix) * vertex_normal;
	vs_color = vertex_color;
	vs_texcoord = vertex_texcoord;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1);
}