//precision mediump float;
uniform sampler2D vTex;
varying vec2 vTexCoord;

void main(void)
{
    vec3 col = texture2D(vTex, vTexCoord).rgb;
    float y = 0.3 *col.r + 0.59 * col.g + 0.11 * col.b;
    gl_FragColor = vec4(y + 0.15, y + 0.07, y - 0.12, 1.0) ;
}
