#version 330

in vec3 fragmentNormal;
in vec3 varColor;
in vec3 cameraDir;
flat in int varType;
out vec4 output;

void main()
{
	vec3 light = normalize(vec3(0, 0, 1));
	vec3 n = normalize(fragmentNormal);

	vec3 ambient = 0.1 * varColor;
	vec3 diffuse = 0.7 * max(dot(light, n), 0) * varColor;

	vec3 r = reflect(light, n);
	float specular = pow(max(dot(normalize(cameraDir), r), 0.0), 20);
	if (varType == 0) {
		output = vec4(ambient + diffuse + vec3(specular, specular, specular), 0.6);
	} else {
		output = vec4(ambient + diffuse + vec3(specular, specular, specular), 1.0);
	}
}