#version 300 es
precision mediump float;

in vec3 v_instanceColor;

uniform vec4 u_ambient;

uniform bool u_points;
uniform bool u_circles;

out vec4 out_color;

void main()
{
	if (u_points == true)
	{
		out_color = vec4(v_instanceColor, 1.0) * u_ambient;
	}
	else if (u_circles == true)
	{
		out_color = vec4(v_instanceColor, 0.25) * u_ambient;
	}
}