#version 120


varying vec2 textureUV;
varying vec3 v;
varying vec3 n;
uniform float hScale;
uniform sampler2D heightTexture;


void main()
{
	v = vec3 (gl_ModelViewMatrix * gl_Vertex);       
	n = vec3 (gl_NormalMatrix * gl_Normal);


	float h = texture2D (heightTexture, gl_MultiTexCoord0.yx).x;
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + vec4 (gl_Normal * h * hScale, 0));
	textureUV = gl_MultiTexCoord0.xy;
}
