

varying vec3 texCoord;
uniform samplerCube cubemap;

void main (void) {
gl_FragColor = textureCube(cubemap, texCoord);
// versiones posteriores simplemente usan "texture(cubemap, texCoord)"
}