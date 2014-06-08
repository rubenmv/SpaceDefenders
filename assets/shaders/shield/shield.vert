#version 120

uniform mat4 mWorldViewProj;
/*
uniform mat4 mInvWorld;
uniform mat4 mTransWorld;
uniform vec3 mLightPos;
uniform vec4 mLightColor;*/

varying vec3 normal, cameraDir;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position = mWorldViewProj * gl_Vertex;
	vec4 PosCoordVista = gl_ModelViewMatrix * gl_Vertex;
	cameraDir = normalize(-vec3(PosCoordVista.xyz));
}
