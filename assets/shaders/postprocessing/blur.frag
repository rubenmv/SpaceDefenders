precision mediump float;
uniform sampler2D vTex;
varying vec2 vTexCoord;

void main(void)
{
    float step = 0.01;
    vec3 c1 = texture2D(vTex, vec2(vTexCoord.s - step, vTexCoord.t - step)).rgb;
    vec3 c2 = texture2D(vTex, vec2(vTexCoord.s + step, vTexCoord.t + step)).rgb;
    vec3 c3 = texture2D(vTex, vec2(vTexCoord.s - step, vTexCoord.t + step)).rgb;
    vec3 c4 = texture2D(vTex, vec2(vTexCoord.s + step, vTexCoord.t - step)).rgb;

    gl_FragColor.a = 1.0;
    //gl_FragColor.rgb = (c1 + c2 + c3 + c4) / 2.0;
    gl_FragColor.rgb = (c1 + c3) / 2.0;
}
