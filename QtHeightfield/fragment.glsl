#version 330

in vec3 fragmentNormal;
in vec3 varColor;
out vec4 output;

void main()
{
	vec3 light = vec3(0, 0, 1);
	vec3 n = max(normalize(fragmentNormal), 0);

	vec3 ambient = 0.3 * varColor;
	vec3 diffuse = 0.7 * dot(light, n) * varColor;
	output = vec4(ambient + diffuse, 1);
}