varying vec3 fragmentNormal;

uniform vec2 center[3];
uniform float frequency[3];
uniform float amplitude[3];
uniform vec2 island;
uniform vec3 lightDir;
uniform float t;
uniform sampler2D texUnit;

void main(void)
{
	fragmentNormal = gl_Normal;

	float height = 0.0;
	vec3 n = vec3(0, 0, 1);

	vec4 pos = gl_Vertex;

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

	gl_Position = gl_ModelViewProjectionMatrix * pos;

}  