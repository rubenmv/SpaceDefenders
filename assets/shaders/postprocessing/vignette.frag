uniform sampler2D vTex;
varying vec2 vTexCoord;

const vec3 SEPIA = vec3(1.1, 1.0, 0.9);

const float SOFTNESS = 0.2;

uniform float u_radius;
uniform float u_intensity;

void main(void)
{
	// Recogemos el color
	vec3 col = texture2D(vTex, vTexCoord).rgb;
	//
	vec2 lensRadius = vec2(u_radius, 0.45);

	float dist = distance(vTexCoord.xy, vec2(0.5,0.5));
	col *= smoothstep(lensRadius.x, lensRadius.y, dist);

	float gray = dot(col.rgb, vec3(0.299, 0.587, 0.114));
	vec3 sepiaColor = vec3(gray) * SEPIA;
	col.rgb = mix(col.rgb, sepiaColor, u_intensity);

	gl_FragColor.a = 1.0f;
	gl_FragColor.rgb =  col;
}
