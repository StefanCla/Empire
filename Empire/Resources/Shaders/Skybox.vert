#version 300 es

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;

uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 v_texturePosition;

void main()
{
	v_texturePosition = in_position;
	vec4 pos = u_projection * u_view * vec4(in_position, 1.0);
	gl_Position = pos.xyww;
}