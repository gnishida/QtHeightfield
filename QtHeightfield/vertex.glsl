#version 330

layout(location = 0)in vec3 vertex;
layout(location = 1)in vec3 color;

out vec3 fragmentNormal;
out vec3 varColor;
out vec3 cameraDir;

uniform mat4 mvpMatrix;
uniform int type;
uniform vec2 center[3];
uniform float frequency[3];
uniform float amplitude[3];
uniform vec2 island;
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
			n.x += amplitude[i] * frequency[i] * cos(dr * frequency[i] - t) * dx / dr * 10;
			n.y += amplitude[i] * frequency[i] * cos(dr * frequency[i] - t) * dy / dr * 10;
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
	vec4 pos = vec4(vertex, 1);
	vec3 n = vec3(0, 0, 1);

	vec2 land_dist = vertex.xy - island;
	float sigma = 0.3;
	float land_height = 0.3 * exp(-dot(land_dist, land_dist) * 0.5 / sigma / sigma);
	pos.z = land_height - 0.22;

	n.x = -sqrt(dot(land_dist, land_dist)) / sigma / sigma * land_height * (vertex.xy - island).x * 10;
	n.y = -sqrt(dot(land_dist, land_dist)) / sigma / sigma * land_height * (vertex.xy - island).y * 10;
	
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
}  