#version 300 es
precision mediump float;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragCoord;

uniform vec3 u_viewPos;
uniform vec4 u_ambient;

uniform bool u_emissiveTex;

uniform sampler2D u_diffuse;
uniform sampler2D u_emissive;

out vec4 out_color;

void main()
{
	vec3 normal = normalize(v_normal);
	vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - v_fragCoord);

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

	// Specular
	float specIntensity = 0.5;
	vec3 viewDir = normalize(u_viewPos - v_fragCoord);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specIntensity * spec * vec3(1.0, 1.0, 1.0);

	// Final fragment color
	float ambientIntensity = 0.35;

	if (u_emissiveTex == true)
		out_color = texture(u_diffuse, v_texCoord) * ((u_ambient * ambientIntensity) + vec4(diffuse, 1.0) + vec4(specular, 1.0))  + texture(u_emissive, v_texCoord);
	else
		out_color = texture(u_diffuse, v_texCoord) * ((u_ambient * ambientIntensity) + vec4(diffuse, 1.0) + vec4(specular, 1.0));
}