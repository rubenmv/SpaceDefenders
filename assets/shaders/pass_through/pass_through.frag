uniform sampler2D fbo_texture;
varying vec2 vTexCoord;

void main(void) {
	gl_FragColor = texture2D(fbo_texture, vTexCoord);
}