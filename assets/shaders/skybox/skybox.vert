
in vec3 vertex;
varying vec3 texCoord;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex, 1.0);
	texCoord = vertex;
}