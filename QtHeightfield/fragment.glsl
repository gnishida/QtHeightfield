varying vec3 fragmentNormal;

void main()
{
	vec3 light = vec3(0, 0, 1);
	vec3 n = max(normalize(fragmentNormal), 0);

	float ambient = 0.3;
	float diffuse = dot(light, n);
	gl_FragColor = vec4(ambient + (1-ambient) * diffuse, ambient + (1-ambient) * diffuse, 1, 0);
}