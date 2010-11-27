
uniform vec3 eyeDir;
varying vec3 N;
varying vec3 P;

void main()
{
	float shinyness = 30.0;
	vec4 ambient = vec4( 0.1, 0.1, 0.1, 0 );

	vec3 L = normalize( gl_LightSource[0].position.xyz - P ); 
	vec3 E = normalize( -eyeDir );
	vec3 R = normalize( -reflect( L, N ) );
	
	float diff = max( dot( N, L ), 0.0 );
	
	vec4 Ispec = vec4( 1, 1, 1, 1 );
	Ispec *= pow( max( dot( R, E ), 0.0 ), shinyness );
	Ispec = clamp(Ispec, 0.0, 1.0); 	

	// Setting Each Pixel To Red
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0)*diff + Ispec;
	
	
	gl_FragColor = gl_Color*diff + Ispec + ambient;
}