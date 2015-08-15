#version 120


varying vec3 n;
varying vec3 v;
varying vec2 textureUV;
uniform sampler2D earthTexture;

void main (void)
{
	vec3 normalizedN = normalize (n);
	vec3 normalizedV = normalize (gl_LightSource[0].position.xyz - v);
	float n_v = max (0.0, dot (normalizedN, normalizedV));
	vec3 diffuse = vec3 (1, 1, 1) * gl_LightSource[0].diffuse.rgb * n_v;
	vec3 r = reflect (-normalizedV, normalizedN);
	float r_v = 0.0;
	if (n_v > 0)
	{
		r_v = pow (max (0.0, dot (r, vec3 (0, 0, 1))), 100);
	}
	vec3 specular = vec3 (1, 1, 1) * gl_LightSource[0].specular.rgb * r_v;
	vec3 ambient = vec3 (0.3, 0.3, 0.3) * gl_LightSource[0].ambient.rgb;
	gl_FragColor = vec4 (ambient + specular + diffuse, 1) * texture2D (earthTexture, textureUV.yx);
}
