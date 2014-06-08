
uniform sampler2D mask;
uniform sampler2D red_tile;
uniform sampler2D green_tile;
uniform sampler2D blue_tile;





void main()
{
vec2 uvCoordMask = vec2(gl_TexCoord[0]);
vec2 uvCoord = vec2(gl_TexCoord[0])*20;

vec4 multRed = texture2D(mask, uvCoordMask).r * texture2D(red_tile, uvCoord);
vec4 multGreen = texture2D(mask, uvCoordMask).g * texture2D(green_tile, uvCoord);
vec4 multBlue = texture2D(mask, uvCoordMask).b * texture2D(blue_tile, uvCoord);


gl_FragColor = multRed+multGreen+multBlue + vec4(0.0156,0.0549, 0.0627,1);
//gl_FragColor = texture2D(blue_tile,vec2(gl_TexCoord[0]));


}
