#version  330  core

uniform  sampler2D  diffuseTex;
uniform  sampler2D  diffuseTex2;
uniform  sampler2D  diffuseTex3;


uniform  vec3    cameraPos;
uniform  vec4    lightColour;
uniform  vec3    lightPos;
uniform  float   lightRadius;


in  Vertex {
vec4  colour;
vec2  texCoord;
vec3  normal;
vec3  worldPos;
float positionY;
} IN;

vec4 getTexture(){

		float minHight = -100.0;
		float maxHight = 900.0;
	
	if (IN.positionY  > minHight && IN.positionY < maxHight){
		
		
		float difference = maxHight-minHight ;
		
		float next = (IN.positionY - minHight)/difference;
		float current = 1.0 - next;
		
		vec4 newTexture = vec4(0.0);
		newTexture = (texture(diffuseTex, IN.texCoord)*current) + (texture(diffuseTex2,IN.texCoord)*next);
		return newTexture;	
	}
	
	else if (IN.positionY < maxHight){
		//vec4 newTexture = vec4(0.0);
		 return texture(diffuseTex3, IN.texCoord);
		//return newTexture;
	}
	
	return texture(diffuseTex, IN.texCoord);
}

out  vec4  fragColour;
void  main(void)    {
     vec3  incident   = normalize(lightPos  - IN.worldPos );
	 vec3  viewDir    = normalize(cameraPos  - IN.worldPos );
	 vec3  halfDir    = normalize(incident + viewDir );
	 
	 vec4  diffuse    = getTexture();
	
	 float  lambert      = max(dot(incident , IN.normal), 0.0f);
	 float  distance     = length(lightPos  - IN.worldPos );
	 float  attenuation = 1.0 - clamp(distance / lightRadius , 0.0,  1.0);
	 float  specFactor   = clamp(dot(halfDir , IN.normal ) ,0.0 ,1.0);
	 
	 specFactor    = pow(specFactor , 90.0 );
	  
	  vec3  surface = (diffuse.rgb * lightColour.rgb);
	  
	  fragColour.rgb = surface * lambert * attenuation;
	  fragColour.rgb += (lightColour.rgb * specFactor )* attenuation *0.33;
	  fragColour.rgb +=  surface * 0.4f;    // ambient!
	  fragColour.a = diffuse.a;
	  
	  ;
}