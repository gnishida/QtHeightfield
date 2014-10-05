#version 330

layout(location = 0)in vec3 vertex;
layout(location = 1)in vec3 color;

out vec3 fragmentNormal;
out vec3 varColor;
out vec3 cameraDir;
flat out int varType;

uniform mat4 mvpMatrix;
uniform int type;
uniform vec2 center[3];
uniform float frequency[3];
uniform float amplitude[3];
uniform vec2 island[2];
uniform vec3 lightDir;
uniform float t;
uniform vec3 cameraPos;
uniform sampler2D texUnit;

void createSea()
{
	varColor = color;

	float height = 0.0;
	vec3 n = vec3(0, 0, 1);

	vec4 pos = vec4(vertex, 1);

	for (int i = 0; i < 3; ++i) {
		float dx = abs(pos.x - center[i].x);
		float dy = abs(pos.y - center[i].y);
		float dr = sqrt(dx * dx + dy * dy);
		height += amplitude[i] * sin(dr * frequency[i] - t);

		if (dr > 0.0) {
			n.x -= amplitude[i] * frequency[i] * cos(dr * frequency[i] - t) * dx / dr * 10;
			n.y -= amplitude[i] * frequency[i] * cos(dr * frequency[i] - t) * dy / dr * 10;
		}
	}
	pos.z = height;
	fragmentNormal = n;
	cameraDir = cameraPos - pos.xyz;

	gl_Position = mvpMatrix * pos;
}

void createIsland()
{
	varColor = color;

	float height = 0.0;
	vec3 n = vec3(0, 0, 1);

	vec4 pos = vec4(vertex, 1);

	for (int i = 0; i < 2; ++i) {
		vec2 land_dist = pos.xy - island[i];
		float sigma = 0.1;
		float e = 0.1 * exp(-dot(land_dist, land_dist) * 0.5 / sigma / sigma);
		height += e;
		n.x += sqrt(dot(land_dist, land_dist)) / sigma / sigma * e * (pos.xy - island[i]).x * 10;
		n.y += sqrt(dot(land_dist, land_dist)) / sigma / sigma * e * (pos.xy - island[i]).y * 10;
	}
	pos.z = height - 0.04;	
	fragmentNormal = n;
	cameraDir = cameraPos - pos.xyz;

	gl_Position = mvpMatrix * pos;
}

void main(void)
{
	if (type == 0) {
		createSea();
	} else {
		createIsland();
	}
	varType = type;
}  