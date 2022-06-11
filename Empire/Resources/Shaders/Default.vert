#version 300 es

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;

layout (location = 3) in vec3 in_instanceColor;
layout (location = 4) in vec3 in_instancePosition;
layout (location = 5) in float in_instanceRadius;

uniform mat4 u_viewProj;

uniform bool u_points;
uniform float u_pointSize;

uniform bool u_circles;
uniform vec3 u_circleColor;

out vec3 v_instanceColor;

void main()
{
	if (u_points == true)
	{
		v_instanceColor = in_instanceColor;
		gl_Position = u_viewProj * vec4(in_position + in_instancePosition, 1.0);
		gl_PointSize = u_pointSize;
	}
	else if (u_circles == true)
	{
		v_instanceColor = u_circleColor;
		vec4 pos = u_viewProj * vec4((in_position * in_instanceRadius) + in_instancePosition, 1.0);
		gl_Position = pos;
	}
}