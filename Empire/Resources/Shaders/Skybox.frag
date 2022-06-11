#version 300 es
precision mediump float;

in vec3 v_texturePosition;

uniform samplerCube u_cube;

out vec4 out_color;

void main()
{
	out_color = texture(u_cube, v_texturePosition) * 0.25;
}