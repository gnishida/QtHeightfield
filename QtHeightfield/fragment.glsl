#version 330

in vec3 fragmentNormal;
in vec3 varColor;
in vec3 cameraDir;
out vec4 output;

void main()
{
	vec3 light = vec3(0, 0, 1);
	vec3 n = max(normalize(fragmentNormal), 0);

	vec3 ambient = 0.1 * varColor;
	vec3 diffuse = 0.7 * dot(light, n) * varColor;

	vec3 r = reflect(light, n);
	float specular = pow(max(dot(normalize(cameraDir), r), 0.0), 20);
	output = vec4(ambient + diffuse + vec3(specular, specular, specular), 1);
}