#version 120


uniform float fIntensity;
uniform float fRadius;
uniform mat4 mMatrixImpact;
uniform sampler2D texture;

varying vec3 normal,cameraDir;

void main()
{


	float resultDot, resultDotInv;

	vec3 n = normalize(normal);

	//Primero calculamos el escudo normal

	resultDotInv = dot(cameraDir, n);
	resultDotInv = (1-pow(resultDotInv, 0.5));

	//cogemos el color de la textura
	vec2 coords = vec2(resultDotInv,0);
	gl_FragColor = texture2D(texture, coords);

	gl_FragColor.a = resultDotInv*fIntensity;

	//Ahora calcularemos el impacto

	//Recorremos las 4 filas de la matriz y lo guardamos
 	for(int i = 0; i<4; i++)
    {

     	//Nos quedamos con la de mayor valor

     	vec4 position = vec4(mMatrixImpact[i][0],mMatrixImpact[i][1],mMatrixImpact[i][2],mMatrixImpact[i][3]);
     	//Solo aplicamos si la intesidad es mayor de 0
     	if(position.w > 0)
     	{
     		resultDot = dot(position.xyz,n);
			if(resultDot > 0)
			{

			    resultDot = pow(resultDot, 2*fRadius);	 //Segun la potencia haremos el impacto mas grande o menos

			}else
			{
			    resultDot = 0;
			}

			//Solo marcaremos como impacto los que sean mas fuertes.
			if(gl_FragColor.a < (resultDot * position.w))
			{
				coords = vec2(resultDot,0);
                vec4 color = texture2D(texture, coords);
               // gl_FragColor = texture2D(texture, coords);
                gl_FragColor = vec4(color.r/2, color.g, color.b, 1);

				gl_FragColor.a = (resultDot * position.w);
			}

     	}


     }
//gl_FragColor.a = 0.5;

}
