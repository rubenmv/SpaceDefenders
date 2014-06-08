#version 120

attribute vec2 v_coord;
uniform sampler2D fbo_texture;
varying vec2 vTexCoord;

void main(void) {
	gl_Position = vec4(v_coord, 0.0, 1.0);
	vTexCoord = (v_coord + 1.0) / 2.0;
}
