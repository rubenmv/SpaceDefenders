

uniform sampler2D vTex;




void main()
{

	/*vec3 col = texture2D(vTex, gl_TexCoord[0])).rgb;
    float y = 0.3 *col.r + 0.59 * col.g + 0.11 * col.b;
    y = y < 0.5 ? 0.0 : 1.0;
    gl_FragColor = vec4(y, y, y, 1.0) ;
*/

vec2 uvCoordMask = vec2(gl_TexCoord[0]);

vec4 color = texture2D(vTex, vec2(uvCoordMask.x,uvCoordMask.y)); 

gl_FragColor = color;
gl_FragColor.r = 0.1;
/*
vec2 uvCoordMask = vec2(gl_TexCoord[0]);
vec2 uvCoord = vec2(gl_TexCoord[0])*20;

vec4 multRed = texture2D(mask, uvCoordMask).r * texture2D(red_tile, uvCoord); 
vec4 multGreen = texture2D(mask, uvCoordMask).g * texture2D(green_tile, uvCoord);
vec4 multBlue = texture2D(mask, uvCoordMask).b * texture2D(blue_tile, uvCoord);


gl_FragColor = multRed+multGreen+multBlue;*/
//gl_FragColor = texture2D(blue_tile,vec2(gl_TexCoord[0]));


}
