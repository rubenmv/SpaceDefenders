precision mediump float;

uniform sampler2D vTex;
varying vec2 vTexCoord;

const float step_w = 0.0015625;
const float step_h = 0.0027778;
const float grid = 1.0;

void main(void)
{
    float offx = floor(vTexCoord.s  / (grid * step_w));
    float offy = floor(vTexCoord.t  / (grid * step_h));

    vec3 res = texture2D(vTex, vec2(offx * grid * step_w , offy * grid * step_h)).rgb;


    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = res;
}
