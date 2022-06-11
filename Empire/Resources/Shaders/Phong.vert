#version 300 es

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;
layout (location = 3) in mat4 in_instanceTransform;

uniform mat4 u_viewProj;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragCoord;

void main()
{
	gl_Position = u_viewProj * in_instanceTransform * vec4(in_position, 1.0);
	v_fragCoord = vec3(in_instanceTransform * vec4(in_position, 1.0));
	v_normal = mat3(transpose(inverse(in_instanceTransform))) * in_normal;
	v_texCoord = in_texCoord;
}