precision mediump float;

uniform sampler2D vTex;
varying vec2 vTexCoord;

const float step_w = 0.0015625;
const float step_h = 0.0027778;

vec3 blur()
{
    float y = vTexCoord.t < 0.4 ? vTexCoord.t : 1.0 - vTexCoord.t;
    float dist = 8.0 - 20.0 * y;

    vec3 acc = vec3(0.0, 0.0, 0.0);
    for (float y=-2.0; y<=2.0; ++y)
    {
        for (float x=-2.0; x<=2.0; ++x)
        {
            acc += texture2D(vTex, vec2(vTexCoord.x + dist * x * step_w, vTexCoord.y + 0.5 * dist * y * step_h)).rgb;
        }
    }

    return acc / 25.0;
}

void main(void)
{
    vec3 col;
    if (vTexCoord.t >= 0.4 && vTexCoord.t <= 0.6)
        col = texture2D(vTex, vTexCoord).rgb;
    else
        col = blur();

    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = col;
}

