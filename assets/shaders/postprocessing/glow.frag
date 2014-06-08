//precision mediump float;

uniform sampler2D vTex;
varying vec2 vTexCoord;

const float step_w = 0.0015625;
const float step_h = 0.0027778;

void main(void)
{
    vec3 t1 = texture2D(vTex, vec2(vTexCoord.x - step_w, vTexCoord.y - step_h)).rgb;
    vec3 t2 = texture2D(vTex, vec2(vTexCoord.x, vTexCoord.y - step_h)).rgb;
    vec3 t3 = texture2D(vTex, vec2(vTexCoord.x + step_w, vTexCoord.y - step_h)).rgb;
    vec3 t4 = texture2D(vTex, vec2(vTexCoord.x - step_w, vTexCoord.y)).rgb;
    vec3 t5 = texture2D(vTex, vTexCoord).rgb;
    vec3 t6 = texture2D(vTex, vec2(vTexCoord.x + step_w, vTexCoord.y)).rgb;
    vec3 t7 = texture2D(vTex, vec2(vTexCoord.x - step_w, vTexCoord.y + step_h)).rgb;
    vec3 t8 = texture2D(vTex, vec2(vTexCoord.x, vTexCoord.y + step_h)).rgb;
    vec3 t9 = texture2D(vTex, vec2(vTexCoord.x + step_w, vTexCoord.y + step_h)).rgb;

    vec3 xx= t1 + 2.0*t2 + t3 - t7 - 2.0*t8 - t9;
    vec3 yy = t1 - t3 + 2.0*t4 - 2.0*t6 + t7 - t9;

    vec3 rr = sqrt(xx * xx + yy * yy);

    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = rr * 2.0 * t5;
}
